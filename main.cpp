#include <curl/curl.h>

#include <iostream>
#include <utility>

#include "Mesh.h"
#include "Mouse.h"
#include "camera.h"
#include "display.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "geometry.h"

static void printGLInfo() {
  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GPU used: " << glGetString(GL_VENDOR) << " - "
            << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes
            << std::endl;
}

static std::unique_ptr<Mesh> constructStateBorder() {
  std::vector<std::vector<Vertex>> polygons =
      readShapefile("res/croatia/CROATIA_HR_Država_ADMIN0.shp");
  std::vector<Vertex> polygon = polygons[0];
  for (auto poly : polygons) {
    if (poly.size() > polygon.size()) {
      polygon = poly;
    }
  }
  unsigned int indices[polygon.size()];
  for (int i = 0; i < polygon.size(); i++) {
    indices[i] = i;
  }
  std::unique_ptr<Mesh> mesh(new Mesh(&polygon[0], polygon.size(), indices,
                                      sizeof(indices) / sizeof(indices[0]),
                                      GL_LINE_STRIP));
  return mesh;
}

int main(int argc, char *argv[]) {
  Display dis(800, 600, "Test one");
  printGLInfo();

  Shader shader("./res/basicShader");
  Transform transform;
  Camera camera(&dis, glm::vec3(16, 46, 1.5), 0.01, 100.0);
  shader.Bind();

  std::unique_ptr<Mesh> mesh = constructStateBorder();
  std::vector<TexturedMesh> dtms;
  for (const auto &entry : fs::directory_iterator("./res/dem")) {
    dtms.push_back(constructMesh(entry.path()));
  }
  while (!dis.isClosed()) {
    dis.Clear(1.0f, 1.0f, 1.0f, 1.0f);
    // TODO put in method code to converting to world coord
    glm::vec3 change = Mouse::get()->getMove();
    if (Mouse::get()->isRotation()) {
      camera.rotate(change);
    } else if (Mouse::get()->isZooming()) {
      camera.zoom(change.x);
    } else if (Mouse::get()->isMove()) {
      double xDelta = 2.0 / 1600.0;
      double yDelta = 2.0 / 1200.0;
      glm::vec3 change = Mouse::get()->getMove();
      change = glm::vec3(-xDelta, yDelta, 0) * change;
      camera.move(change);
    }
    shader.UpdateTransform(transform, camera);

    mesh->Draw();
    for (std::vector<TexturedMesh>::const_iterator it = dtms.begin();
         it != dtms.end(); it++) {
      it->second->Bind(0);
      it->first->Draw();
    }
    dis.Update();
  }

  return 0;
}