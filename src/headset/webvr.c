#include "headset/headset.h"
#include "event/event.h"
#include "graphics/graphics.h"
#include "lib/vec/vec.h"
#include <stdbool.h>

// Provided by resources/lovr.js
extern void webvrInit(void);
extern void webvrSetCallbacks(void (*added)(uint32_t id), void (*removed)(uint32_t id), void (*pressed)(uint32_t, ControllerButton button), void (*released)(uint32_t id, ControllerButton button), void (*mount)(bool mounted));
extern HeadsetType webvrGetType(void);
extern HeadsetOrigin webvrGetOriginType(void);
extern bool webvrIsMounted(void);
extern bool webvrIsMirrored(void);
extern void webvrSetMirrored(bool mirror);
extern void webvrGetDisplayDimensions(int32_t* width, int32_t* height);
extern void webvrGetClipDistance(float* near, float* far);
extern void webvrSetClipDistance(float near, float far);
extern void webvrGetBoundsDimensions(float* width, float* depth);
extern void webvrGetPose(float* x, float* y, float* z, float* angle, float* ax, float* ay, float* az);
extern void webvrGetEyePose(HeadsetEye eye, float* x, float* y, float* z, float* angle, float* ax, float* ay, float* az);
extern void webvrGetVelocity(float* x, float* y, float* z);
extern void webvrGetAngularVelocity(float* x, float* y, float* z);
extern bool webvrControllerIsConnected(Controller* controller);
extern ControllerHand webvrControllerGetHand(Controller* controller);
extern void webvrControllerGetPose(Controller* controller, float* x, float* y, float* z, float* angle, float* ax, float* ay, float* az);
extern float webvrControllerGetAxis(Controller* controller, ControllerAxis axis);
extern bool webvrControllerIsDown(Controller* controller, ControllerButton button);
extern bool webvrControllerIsTouched(Controller* controller, ControllerButton button);
extern void webvrControllerVibrate(Controller* controller, float duration, float power);
extern ModelData* webvrControllerNewModelData(Controller* controller);
extern void webvrRenderTo(void (*callback)(void*), void* userdata);
extern void webvrUpdate(float dt);

typedef struct {
  float offset;
  vec_controller_t controllers;
} HeadsetState;

static HeadsetState state;

static void onControllerAdded(uint32_t id) {
  Controller* controller = lovrAlloc(sizeof(Controller), free);
  controller->id = id;
  vec_push(&state.controllers, controller);
  lovrRetain(controller);
  lovrEventPush((Event) {
    .type = EVENT_CONTROLLER_ADDED,
    .data = { .controlleradded = { controller } }
  });
}

static void onControllerRemoved(uint32_t id) {
  for (int i = 0; i < state.controllers.length; i++) {
    if (state.controllers.data[i]->id != id) {
      continue;
    }

    Controller* controller = state.controllers.data[i];
    lovrRetain(controller);
    lovrEventPush((Event) {
      .type = EVENT_CONTROLLER_REMOVED,
      .data = { .controllerremoved = { controller } }
    });
    vec_splice(&state.controllers, i, 1);
    lovrRelease(controller);
    break;
  }
}

static void onControllerPressed(uint32_t id, ControllerButton button) {
  lovrEventPush((Event) {
    .type = EVENT_CONTROLLER_PRESSED,
    .data = { .controllerpressed = { state.controllers.data[id], button } }
  });
}

static void onControllerReleased(uint32_t id, ControllerButton button) {
  lovrEventPush((Event) {
    .type = EVENT_CONTROLLER_RELEASED,
    .data = { .controllerreleased = { state.controllers.data[id], button } }
  });
}

static void onMountChanged(bool mounted) {
  lovrEventPush((Event) {
    .type = EVENT_MOUNT,
    .data = { .mount = { mounted } }
  });
}

static bool webvrDriverInit(float offset) {
  state.offset = offset;
  vec_init(&state.controllers);
  webvrInit();
  webvrSetCallbacks(onControllerAdded, onControllerRemoved, onControllerPressed, onControllerReleased, onMountChanged);
  return true;
}

static void webvrDriverDestroy() {
  vec_deinit(&state.controllers);
  memset(&state, 0, sizeof(HeadsetState));
}

Controller** webvrGetControllers(uint8_t* count) {
  *count = state.controllers.length;
  return state.controllers.data;
}

HeadsetInterface lovrHeadsetWebVRDriver = {
  DRIVER_WEBVR,
  webvrDriverInit,
  webvrDriverDestroy,
  webvrGetType,
  webvrGetOriginType,
  webvrIsMounted,
  webvrIsMirrored,
  webvrSetMirrored,
  webvrGetDisplayDimensions,
  webvrGetClipDistance,
  webvrSetClipDistance,
  webvrGetBoundsDimensions,
  webvrGetPose,
  webvrGetEyePose,
  webvrGetVelocity,
  webvrGetAngularVelocity,
  webvrGetControllers,
  webvrControllerIsConnected,
  webvrControllerGetHand,
  webvrControllerGetPose,
  webvrControllerGetAxis,
  webvrControllerIsDown,
  webvrControllerIsTouched,
  webvrControllerVibrate,
  webvrControllerNewModelData,
  webvrRenderTo,
  webvrUpdate
};
