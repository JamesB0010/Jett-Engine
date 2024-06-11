#pragma once
#include <SDL.h>
#include <functional>

struct InputCallback {
	virtual void runCallback() {};
};

template <typename T>
struct MemberFuncKeyListener : public InputCallback {
private:
	T* memberFuncObject = nullptr;
	std::function<void(T*)> callback;


public:
	MemberFuncKeyListener(T* obj, std::function<void(T*)> callback)
		: callback(callback), memberFuncObject(obj) {};

	void runCallback() override{
		callback(memberFuncObject);
	}
};

struct FreeFuncKeyListener : public InputCallback {
private:
	std::function<void()> callback;

public:
	FreeFuncKeyListener(std::function<void()> callback)
		: callback(callback) {};

	void runCallback()override {
		callback();
	}
};

template <typename T>
struct MemberFuncButtonListener : public InputCallback {
private:
	T* memberFuncObject = nullptr;
	std::function<void(T*)> callback;


public:
	MemberFuncButtonListener(T* instance, std::function<void(T*)> callback)
		: callback(callback), memberFuncObject(instance) {};

	void runCallback() override {
		callback(memberFuncObject);
	}
};

struct FreeFuncButtonListener : public InputCallback {
private:
	std::function<void()> callback;

public:
	FreeFuncButtonListener(std::function<void()> callback)
		: callback(callback) {};

	void runCallback()override {
		callback();
	}
};

