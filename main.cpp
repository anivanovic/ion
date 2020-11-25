#include "display.h"
#include "Mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "Mouse.h"
#include <ogrsf_frmts.h>
#include <iostream>
#include <curl/curl.h>

std::vector<std::vector<Vertex>> parseMulty(OGRMultiPolygon *multy)
{
    int nGeoms = multy->getNumGeometries();
    std::vector<std::vector<Vertex>> layers;
    layers.reserve(nGeoms);

    for (auto *geom : multy)
    {
        OGRLinearRing *ring = geom->getExteriorRing();
        int nPoints = ring->getNumPoints();
        std::cout << nPoints << std::endl;
        std::vector<Vertex> points;
        points.reserve(nPoints);
        OGRPoint point;
        for (int i = 0; i < nPoints; i++)
        {
            ring->getPoint(i, &point);
            Vertex vex(glm::vec3(point.getX(), point.getY(), point.getZ()), glm::vec2(0, 0));
            points.push_back(vex);
        }
        layers.push_back(points);
    }

    return layers;
}

std::vector<std::vector<Vertex>> readShape()
{
    GDALAllRegister();
    GDALDataset *poDS = (GDALDataset *)GDALOpenEx("res/croatia/CROATIA_HR_Država_ADMIN0.shp", GDAL_OF_VECTOR, NULL, NULL, NULL);

    if (poDS == NULL)
    {
        std::cout << "Open failed." << std::endl;
        exit(1);
    }
    int count = poDS->GetLayerCount();

    for (int i = 0; i < count; i++)
    {
        OGRLayer *layer = poDS->GetLayer(i);
        std::cout << layer->GetName() << std::endl;
    }

    OGRLayer *layer = poDS->GetLayer(0);
    count = layer->GetFeatureCount();
    std::cout << count << std::endl;
    std::vector<std::vector<Vertex>> points;
    for (auto &feature : layer)
    {
        OGRGeometry *geom = feature->GetGeometryRef();
        std::cout << OGRGeometryTypeToName(geom->getGeometryType()) << std::endl;
        OGRMultiPolygon *poly = geom->toMultiPolygon();

        points = parseMulty(poly);
    }

    return points;
}

int main(int argc, char *argv[])
{
    Display dis(800, 600, "Test one");

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
    std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GPU used: " << glGetString(GL_VENDOR) << " - " << glGetString(GL_RENDERER) << std::endl;

    std::vector<std::vector<Vertex>> polygons = readShape();
    GDALDataset* poDataset = (GDALDataset *) GDALOpen("./res/dem/N46E016.hgt", GA_ReadOnly);
    std::cout << poDataset->GetDriver()->GetDescription() << std::endl;
    std::cout << "Number of bands: " << poDataset->GetBands().size() << std::endl;
    std::cout << "Pixel width: " << poDataset->GetRasterXSize() << ", height: " << poDataset->GetRasterYSize() << std::endl;
    std::cout << "Projectio is: " << poDataset->GetProjectionRef() << std::endl;
    double afin[6];
    poDataset->GetGeoTransform(afin);
    #define print(a) a << ", "
    std::cout << print(afin[0]) << print(afin[1]) << print(afin[2]) << print(afin[3]) << print(afin[4]) << print(afin[5]) << std::endl;
    #undef print

    double xOrigin = afin[0];
    double yOrigin = afin[3];
    double xDelta = afin[1];
    double yDelta = afin[5];

    GDALRasterBand* band = poDataset->GetRasterBand(1);
    int nBlockXSize, nBlockYSize;
    band->GetBlockSize(&nBlockXSize, &nBlockYSize);
    int xSize = band->GetXSize();
    int ySize = band->GetYSize();
    std::cout << "x size " << xSize << ", y size " << ySize << std::endl;
    float* pafScanline = (float *) CPLMalloc(sizeof(float) * xSize);

    std::vector<Vertex> dtmVertices;
    for (int i = 0; i < ySize; i += 100) {
        band->RasterIO(GF_Read, 0, i, xSize, 1,
            pafScanline, xSize, 1, GDT_Float32,
            0, 0);
        double yCoord = yOrigin + (yDelta * i);
        double t = (yCoord - 45.0) / 2.0;

        for (int j = 0; j < xSize; j += 100) {
            double xCoord = xOrigin + (xDelta * j);
            // TODO calculation of tex coordinates should be apstracted away
            double s = (xCoord - 14.6667) / 2.6666;
            // 0.00001 is ratio of one wgs degree to metars (1/108000)
            // We transform heights in metars to degrese here.
            double height = pafScanline[j] * 0.00001;
            Vertex vex(glm::vec3(xCoord, yCoord, height), glm::vec2(s, t));
            dtmVertices.push_back(vex);
        }
    }

    std::vector<unsigned int> dtmIndices;
    for (int i = 0; i < (ySize / 100); i++) {
        for (int j = 0; j < (xSize / 100) - 1; j++) {
            int rowSize = (xSize / 100) + 1;
            int xOffset = (i * rowSize) + j;

            int first = xOffset;
            int second = xOffset + rowSize;
            int third = xOffset + 1;
            int fourth = xOffset + rowSize + 1;

            #define push(x) if (x >= dtmVertices.size()) std::cout << "Pushing indx out of bound " << x << std::endl; dtmIndices.push_back(x);
            push(first);
            push(second);
            push(third);
            push(third);
            push(second);
            push(fourth);
            std::cout << "first triag " << first << " " << second << " " << third << std::endl;
            std::cout << "second triag " << first << " " << third << " " << fourth << std::endl;
            #undef push;
        }
    }
    Mesh dtmMesh(&dtmVertices[0], dtmVertices.size(), &dtmIndices[0], dtmIndices.size(), GL_TRIANGLES);


    std::vector<Vertex> polygon = polygons[0];
    for (auto poly : polygons)
    {
        if (poly.size() > polygon.size())
        {
            polygon = poly;
        }
    }
    unsigned int indices[polygon.size()];
    for (int i = 0; i < polygon.size(); i++) {
        indices[i] = i;
    }
    Mesh mesh(&polygon[0], polygon.size(), indices, sizeof(indices)/sizeof(indices[0]), GL_LINE_STRIP);
    Shader shader("./res/basicShader");

    Texture tex("./res/wms2.png");
    Transform transform;
    Camera camera(&dis, glm::vec3(16, 46, 1.5), 0.01, 100.0);

    transform.GetScale() = glm::vec3(1, 1, 1);
    while (!dis.isClosed())
    {
        dis.Clear(1.0f, 1.0f, 1.0f, 1.0f);
        if (Mouse::get()->isActive())
        {
            // TODO put in method code to converting to world coord
            glm::vec3 change = Mouse::get()->getMove();
            if (Mouse::get()->isRotation()) {
                camera.rotate(change);
            } else {
                double xDelta = 2.0 / 1600.0;
                double yDelta = 2.0 / 1200.0;
                change = glm::vec3(-xDelta, yDelta, 0) * change;
                camera.move(change);
            }
        }
        shader.Bind();
        shader.UpdateTransform(transform, camera);
        mesh.Draw();
        tex.Bind(0);
        dtmMesh.Draw();
        dis.Update();
    }
    return 0;
}