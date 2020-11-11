#include "display.h"
#include "Mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "Mouse.h"
#include <ogrsf_frmts.h>
#include <iostream>

std::vector<std::vector<Vertex>> parseMulty(OGRMultiPolygon* multy) {
    int nGeoms = multy->getNumGeometries();
    std::vector<std::vector<Vertex>> layers;
    layers.reserve(nGeoms);

    for (auto* geom : multy) {
        OGRLinearRing* ring = geom->getExteriorRing();
        int nPoints = ring->getNumPoints();
        std::cout << nPoints << std::endl;
        std::vector<Vertex> points;
        points.reserve(nPoints);
        OGRPoint point;
        for (int i = 0; i < nPoints; i++){
            ring->getPoint(i, &point);
            Vertex vex(glm::vec3(point.getX(), point.getY(), point.getZ()), glm::vec2(0, 0));
            points.push_back(vex);
        }
        layers.push_back(points);
    }

    return layers;
}

std::vector<std::vector<Vertex>> readShape() {
    GDALAllRegister();
    GDALDataset* poDS = (GDALDataset*) GDALOpenEx("res/croatia/CROATIA_HR_Država_ADMIN0.shp", GDAL_OF_VECTOR, NULL, NULL, NULL);

    if(poDS == NULL)
    {
        std::cout << "Open failed." << std::endl;
        exit(1);
    }
    int count = poDS->GetLayerCount();

    for (int i = 0; i < count; i++) {
        OGRLayer* layer = poDS->GetLayer(i);
        std::cout << layer->GetName() << std::endl;
    }

    OGRLayer* layer = poDS->GetLayer(0);
    count = layer->GetFeatureCount();
    std::cout << count << std::endl;
    std::vector<std::vector<Vertex>> points;
    for (auto& feature : layer) {
        OGRGeometry* geom = feature->GetGeometryRef();
        std::cout << OGRGeometryTypeToName(geom->getGeometryType()) << std::endl;
        OGRMultiPolygon* poly = geom->toMultiPolygon();

        points = parseMulty(poly);
    }

    return points;
}

int main(int argc, char *argv[]) {
    std::vector<std::vector<Vertex>> polygons = readShape();
    Display dis(800, 600, "Test one");
    Shader shader("./res/basicShader");

    std::vector<Vertex> polygon = polygons[0];
    for (auto poly : polygons) {
        if (poly.size() > polygon.size()) {
            polygon = poly;
        }
    }
    Mesh mesh(&polygon[0], polygon.size());
    // Mesh mesh(vertices, sizeof(vertices)/sizeof(vertices[0]));
    // Texture tex("./res/my.jpg");
    Transform transform;
    Camera camera(&dis, glm::vec3(16, 46, 4), 0.01, 100.0);

    float counter = 0.0f;
    transform.GetScale() = glm::vec3(1, 1, 1);
    while(!dis.isClosed()) {
        dis.Clear(1.0f, 1.0f, 1.0f, 1.0f);

        // transform.GetPos().x = sinf(counter);
        // transform.GetRot().z = counter;
        // transform.GetScale() = (transform.GetScale() / transform.GetScale()) * cosf(counter);

        shader.Bind();
        // tex.Bind(0);
        if (Mouse::get()->isActive()) {
            glm::vec3 change = Mouse::get()->getMove();
            camera.move(change);
        }
        shader.UpdateTransform(transform, camera);
        mesh.Draw();

        dis.Update();
        counter += 0.01f;
    }

    return 0;
}