#include "core/scenery/Scenery.h"

Scenery::Scenery() {
  is_coord_world = true;
}

Scenery::~Scenery() {
}

/*----- Object methods -----*/
void Scenery::addObj(Object* obj) {
  objs.push_back(obj);
}
//  void delObj(Object obj);
//  void findObj(Object obj);

// Return as pointer to the object
Object* Scenery::getObj(int i) {
  if(i >= objs.size())
    return NULL;

  std::list<Object*>::iterator it = objs.begin();
  std::advance(it, i);
  return *it;
}

void Scenery::applyTransformAll(const TMatrix & matrix) {
  std::list<Object*>::iterator it_objs;
  for(it_objs = objs.begin(); it_objs != objs.end(); ++it_objs){
    (*it_objs)->applyTransform(matrix);
  }

  std::list<Light*>::iterator it_lights;
  for(it_lights = lights.begin(); it_lights != lights.end(); ++it_lights){
    (*it_lights)->applyTransform(matrix);
  }
}

/*----- Ray Intersection -----*/
// TODO: handle this code in a separete function. this will be useful
// in models which cointain more than one object.
// Object hitObject must also return a material or a reference to the
// hitted object.
float Scenery::hitRay(Vertex3f ray, Material & mat, Vertex3f & n) {
  Vertex3f normal; // Normal of the hitted face
  Vertex3f best_normal; // Normal of the hitted face
  Material* first_mat;
  float best_t = hitObjectList(objs, ray, first_mat, n);


  /*
  std::list<Object*>::iterator it;
  for(it = objs.begin(); it != objs.end(); ++it){
    float t = (*it)->hitObject(ray, normal);
    if(t <= best_t && t >= 1.0) {
      best_t = t;
      best_normal = normal;
      first_mat = (*it)->getMaterial();
    }
  }
// TODO: Compare the result of this code with the current
// method implemented in RayCasting.cpp
  if(best_t < FLT_MAX) {
    n = best_normal.unit();
    mat = *first_mat;
  }
  */
  if(best_t < FLT_MAX) {
    mat = *first_mat;
  }

  return best_t;
}

/*----- Light sources methods -----*/
void Scenery::addLight(Light* source) {
  lights.push_back(source);
}

Light* Scenery::getLight(int i) {
  if(i >= objs.size())
    return NULL;

  std::list<Light*>::iterator it = lights.begin();
  std::advance(it, i);
  return *it;
}

/*----- Camera Methods -----*/
void Scenery::setCamPos(Vertex3f position) { cam.setPosition(position); }
void Scenery::setCamLookAt(Vertex3f position) { cam.setLookAt(position); }
void Scenery::setCamAViewUp(Vertex3f position) { cam.setAViewUp(position); }
void Scenery::calcCamCoord() { cam.calcCoordSystemBasis(); }

Camera* Scenery::getCam() { return &cam; }

/*----- Coordinates transformations -----*/
bool Scenery::isCoordWorld() {
  return is_coord_world;
}

void Scenery::worldToCamTransform() {
  if(this->isCoordWorld()) {
    TMatrix transform = getWorldToCamTransform();
    this->applyTransformAll(transform);
    is_coord_world = true;
  }
}

void Scenery::camToWorldTransform() {
  if(!this->isCoordWorld()) {
    TMatrix transform = getCamToWorldTransform();
    this->applyTransformAll(transform);
    is_coord_world = false;
  }
}

TMatrix Scenery::getWorldToCamTransform() {
  TMatrix transform;
  Vertex3f i = cam.getI();
  Vertex3f j = cam.getJ();
  Vertex3f k = cam.getK();
  Vertex3f pos = cam.getPosition();

  transform.worldBasisToCoord(i, j, k, pos);
  return transform;
}

TMatrix Scenery::getCamToWorldTransform() {
  TMatrix transform;
  Vertex3f i = cam.getI();
  Vertex3f j = cam.getJ();
  Vertex3f k = cam.getK();
  Vertex3f pos = cam.getPosition();

  transform.coordBasisToWorld(i, j, k, pos);

  return transform;
}
