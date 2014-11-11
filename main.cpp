#include <iostream>
#include <random>
#include "time.h"
#include "entity.h"
#include <string>
#include <vector>
#include "Application.h"
#define PI 3.14159265358979323846
using namespace std;
SDL_Window* displayWindow;

int main(int argc, char *argv[]) {
	Application app;
	while (!app.UpdateAndRender()) {}
	return 0;
}