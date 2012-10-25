#include "magickWandJs.h"

Handle<Value> Async(const Arguments &args){
	
	// assume that args[0] is the callbak
	if(!args[0]->IsFunction()){
		return ThrowException(Exception::TypeError(
			String::New("callback function expected")));
	}

	Local<Function> callback = Local<Function>::Cast(args[0]);

	Baton *baton = new Baton();

	baton->error = false;
	baton->request.data = baton;
	baton->callback = Persistent<Function>::New(callback);
	
	// schedule the work request in libuv queue
	int status = uv_queue_work(uv_default_loop(), &baton->request, AsyncWork, AsyncAfter);
	assert(status == 0);

	return Undefined();
}

void AsyncWork(uv_work_t *req){
	Baton *baton = static_cast<Baton*>(req->data);
	
	baton->result = 1337;
}

void AsyncAfter(uv_work_t *req){
	HandleScope scope;

	Baton *baton = static_cast<Baton*>(req->data);

	if(baton->error){
		Local<Value> err = Exception::Error(String::New(baton->error_message.c_str()));

		const unsigned argc = 1;

		// create array of length of argc
		Local<Value> argv[argc] = { err };

		TryCatch try_catch;
		
		// call the callback with the error
		baton->callback->Call(Context::GetCurrent()->Global(), argc, argv);
		if(try_catch.HasCaught()){
			node::FatalException(try_catch);
		}

	} else {
		const unsigned argc = 2;

		Local<Value> argv[argc] = {
			Local<Value>::New(Null()),
			Local<Value>::New(Integer::New(baton->result))
		};

		TryCatch try_catch;
		baton->callback->Call(Context::GetCurrent()->Global(), argc, argv);
		if(try_catch.HasCaught()){
			node::FatalException(try_catch);
		}
	}

	baton->callback.Dispose();
	delete baton;

}

void test(){
	MagickWand *wand = NULL;

	MagickWandGenesis();
}

void MagickWandJS(Handle<Object> target){
	target->Set(String::NewSymbol("async"),
		FunctionTemplate::New(Async)->GetFunction());
}

NODE_MODULE(magickWandJs, MagickWandJS);
