// -- nodejs and v8 libs
#include <node/v8.h>
#include <node/node.h>

// -- imagemagick stuff
#include <wand/magick_wand.h>

#include <string>


using namespace v8;


Handle<Value> Async(const Arguments &args);
void AsyncWork(uv_work_t *req);
void AsyncAfter(uv_work_t *req);

struct Baton {
	// libuv request struct
	uv_work_t request;

	// holds the callback function
	Persistent<Function> callback;

	// error handling
	bool error;
	std::string error_message;
	
	// some value to modify
	int32_t result;
};



Handle<Value> ResizeAsync(const Arguments &args);
void ResizeAsyncWork(uv_work_t *req);
void ResizeAsyncAfter(uv_work_t *req);

struct ResizeBaton {
	uv_work_t request;
	Persistent<Function> callback;
	
	bool error;
};
