#pragma once

#include <ogrsf_frmts.h>

#include <filesystem>
#include <iostream>

#include "Mesh.h"
#include "texture.h"

namespace fs = std::__fs::filesystem;

typedef std::pair<std::unique_ptr<Mesh>, std::unique_ptr<Texture>> TexturedMesh;

static void GDALInit() { GDALAllRegister(); }

std::vector<std::vector<Vertex>> parseMultyPoligon(OGRMultiPolygon *multy) {
  int nGeoms = multy->getNumGeometries();
  std::vector<std::vector<Vertex>> layers;
  layers.reserve(nGeoms);

  for (auto *geom : multy) {
    OGRLinearRing *ring = geom->getExteriorRing();
    int nPoints = ring->getNumPoints();
    std::cout << nPoints << std::endl;
    std::vector<Vertex> points;
    points.reserve(nPoints);
    OGRPoint point;
    for (int i = 0; i < nPoints; i++) {
      ring->getPoint(i, &point);
      Vertex vex(glm::vec3(point.getX(), point.getY(), point.getZ()),
                 glm::vec2(0, 0));
      points.push_back(vex);
    }
    layers.push_back(points);
  }

  return layers;
}

std::vector<std::vector<Vertex>> readShapefile(const std::string &filepath) {
  GDALDataset *poDS = (GDALDataset *)GDALOpenEx(
      filepath.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL);

  if (poDS == NULL) {
    std::cout << "Open failed." << std::endl;
    exit(1);
  }
  int count = poDS->GetLayerCount();

  for (int i = 0; i < count; i++) {
    OGRLayer *layer = poDS->GetLayer(i);
    std::cout << layer->GetName() << std::endl;
  }

  OGRLayer *layer = poDS->GetLayer(0);
  count = layer->GetFeatureCount();
  std::cout << count << std::endl;
  std::vector<std::vector<Vertex>> points;
  for (auto &feature : layer) {
    OGRGeometry *geom = feature->GetGeometryRef();
    std::cout << OGRGeometryTypeToName(geom->getGeometryType()) << std::endl;
    OGRMultiPolygon *poly = geom->toMultiPolygon();

    points = parseMultyPoligon(poly);
  }

  return points;
}

TexturedMesh constructMesh(fs::path filepath) {
  GDALDataset *poDataset =
      (GDALDataset *)GDALOpen(filepath.c_str(), GA_ReadOnly);
  double afin[6];
  poDataset->GetGeoTransform(afin);

  double xOrigin = afin[0];
  double yOrigin = afin[3];
  double xDelta = afin[1];
  double yDelta = afin[5];

  GDALRasterBand *band = poDataset->GetRasterBand(1);
  int xSize = band->GetXSize();
  int ySize = band->GetYSize();
  float *pafScanline = (float *)CPLMalloc(sizeof(float) * xSize);
  double xEnd = (xSize - 1) * xDelta + xOrigin;
  double yEnd = (ySize - 1) * yDelta + yOrigin;

  std::vector<Vertex> dtmVertices;
  for (int i = 0; i < ySize; i += 100) {
    band->RasterIO(GF_Read, 0, i, xSize, 1, pafScanline, xSize, 1, GDT_Float32,
                   0, 0);
    double yCoord = yOrigin + (yDelta * i);
    double t = (yCoord - yEnd) / std::abs((ySize - 1) * yDelta);

    for (int j = 0; j < xSize; j += 100) {
      double xCoord = xOrigin + (xDelta * j);
      // TODO calculation of tex coordinates should be abstracted away
      double s = (xCoord - xOrigin) / ((xSize - 1) * xDelta);
      // 0.00001 is ratio of one wgs degree to metars (1/108000)
      // We transform heights in metars to degrese here.
      double height = pafScanline[j] * 0.00009;
      Vertex vex(glm::vec3(xCoord, yCoord, height), glm::vec2(s, t));
      dtmVertices.push_back(vex);
    }
  }

  std::vector<unsigned int> dtmIndices;
  for (int i = 0; i < (ySize / 100); i++) {
    for (int j = 0; j < (xSize / 100); j++) {
      int rowSize = (xSize / 100) + 1;
      int xOffset = (i * rowSize) + j;

      int first = xOffset;
      int second = xOffset + rowSize;
      int third = xOffset + 1;
      int fourth = xOffset + rowSize + 1;

#define push(x)                                                  \
  if (x >= dtmVertices.size())                                   \
    std::cout << "Pushing indx out of bound " << x << std::endl; \
  dtmIndices.push_back(x);
      push(first);
      push(second);
      push(third);
      push(third);
      push(second);
      push(fourth);
#undef push
    }
  }

  CPLFree(pafScanline);
  std::string filename = filepath.filename();
  // remove file extension
  int lastIndex = filename.find_last_of(".");
  filename = filename.substr(0, lastIndex);
  fs::path wmsFolder = filepath.parent_path().parent_path().append("wms");
  std::string texturePath = wmsFolder.append(filename + ".png");
  std::unique_ptr<Texture> tex(new Texture(texturePath));
  std::unique_ptr<Mesh> mesh(new Mesh(&dtmVertices[0], dtmVertices.size(),
                                      &dtmIndices[0], dtmIndices.size(),
                                      GL_TRIANGLES));
  return std::make_pair(std::move(mesh), std::move(tex));
}