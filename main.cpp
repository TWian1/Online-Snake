#include <SDL.h>
#undef main
#include <iostream>
#include <random>
#include <chrono>
#include <string> 
#include <cstring>
#include <vector>
#include <asio.hpp>
#include <vector>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#define ASIO_STANDALONE
using namespace std;
int main(int argx, char* argv[])
{	// -------------------------
	int displaysizex = 1800;
	int displaysizey = 1000;
	int xsize = 90; // In Pixels
	int ysize = 50;
	int delay = 1; // In Ms
	bool infiniteGenerations = true;
	int generationsize = 100; // number of generations
	double framecap = 15.0; // The lower the framecap the greater the performance
	string ipadress = "127.0.0.1";
	// -------------------------
	vector<vector<int>> snake = { {xsize / 2, ysize / 2} };
	vector<vector<int>> osnake;
	int foodx, foody;
	int dir = 0;
	string output, noutput;
	cout << "Username(_ for spaces and no special characters): ";
	cin >> noutput;
	asio::error_code ec;
	asio::io_context context;
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address(ipadress, ec), 6873);
	asio::ip::tcp::socket socket(context);
	socket.connect(endpoint, ec);
	if (socket.is_open()) {
		string sRequest ="GET /u>" + noutput + "\r\n""Connection: close\r\n\r\n";
		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);
		socket.wait(socket.wait_read);}
	string testmsg = "test";
	vector<vector<string>> others;
	vector<vector<int>> other;
	bool ate = false;
	int curdirr = dir;
	SDL_Event e;
	srand(time(NULL));
	random_device rd;
	mt19937 gen(rd());
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, displaysizex, displaysizey, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	double* out = new double[xsize * ysize];
	for (int y1 = 0; y1 < (xsize * ysize); y1++) {out[y1] = 0;}
	SDL_RenderSetScale(renderer, static_cast<float>(floor(displaysizex / xsize)), static_cast<float>(floor(displaysizey / ysize)));
	auto start2 = chrono::high_resolution_clock::now();
	uniform_int_distribution<> distribution2(0, xsize - 1);
	foodx = distribution2(gen);
	uniform_int_distribution<> distribution3(0, ysize - 1);
	foody = distribution3(gen);
	out[(foody * xsize) + foodx] = 2;
	int generation = 0;
	if (infiniteGenerations == true) { generationsize = -1; }
	for (int snkpix = 0; snkpix < snake.size(); snkpix++) {out[(snake[snkpix][1] * xsize) + snake[snkpix][0]] = 1;}
	while (generation != generationsize) {
		generation++;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {generation = generationsize;}
			else if (e.type == SDL_KEYDOWN) {
				if (SDLK_RIGHT == e.key.keysym.sym && curdirr != 2) { dir = 1; }
				else if (SDLK_LEFT == e.key.keysym.sym && curdirr != 1) { dir = 2; }
				else if (SDLK_DOWN == e.key.keysym.sym && curdirr != 4) { dir = 3; }
				else if (SDLK_UP == e.key.keysym.sym && curdirr != 3) { dir = 4; }
				else if (SDLK_ESCAPE == e.key.keysym.sym) {
					asio::error_code ec;
					asio::io_context context;
					asio::ip::tcp::endpoint endpoint(asio::ip::make_address(ipadress, ec), 6873);
					asio::ip::tcp::socket socket(context);
					socket.connect(endpoint, ec);
					if (socket.is_open()) {
						string sRequest ="GET /quit\r\n""Connection: close\r\n\r\n";
						socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);
						socket.wait(socket.wait_read);
						return 0;}}}}
		auto finish = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed2 = finish - start2;
		if (elapsed2.count() > (1.0 / framecap)) {
			curdirr = dir;
			start2 = chrono::high_resolution_clock::now();
			osnake = snake;
			if (dir == 1) {
				if (snake[snake.size() - 1][0] + 1 > xsize - 1) {snake.clear(); snake.push_back({ xsize / 2, ysize / 2 }); dir = 0; ate = true; out[(foody * xsize) + foodx] = 0; uniform_int_distribution<> distribution2(0, xsize - 1); foodx = distribution2(gen); uniform_int_distribution<> distribution3(0, ysize - 1); foody = distribution3(gen); out[(foody * xsize) + foodx] = 2;
				}
				else { snake.push_back({ snake[snake.size() - 1][0] + 1, snake[snake.size() - 1][1] }); }}
			else if (dir == 2) {
				if (snake[snake.size() - 1][0] - 1 < 0) {snake.clear(); snake.push_back({ xsize / 2, ysize / 2 }); dir = 0; ate = true; out[(foody * xsize) + foodx] = 0; uniform_int_distribution<> distribution2(0, xsize - 1); foodx = distribution2(gen); uniform_int_distribution<> distribution3(0, ysize - 1); foody = distribution3(gen); out[(foody * xsize) + foodx] = 2;
				}
				else {snake.push_back({ snake[snake.size() - 1][0] - 1, snake[snake.size() - 1][1] });}}
			else if (dir == 3) {
				if (snake[snake.size() - 1][1] + 1 > ysize - 1) {snake.clear(); snake.push_back({ xsize / 2, ysize / 2 }); dir = 0; ate = true; out[(foody * xsize) + foodx] = 0; uniform_int_distribution<> distribution2(0, xsize - 1); foodx = distribution2(gen); uniform_int_distribution<> distribution3(0, ysize - 1); foody = distribution3(gen); out[(foody * xsize) + foodx] = 2;
				}
				else {snake.push_back({ snake[snake.size() - 1][0], snake[snake.size() - 1][1] + 1 });}}
			else if (dir == 4) {
				if (snake[snake.size() - 1][1] - 1 < 0) { snake.clear(); snake.push_back({ xsize / 2, ysize / 2 }); dir = 0; ate = true; out[(foody * xsize) + foodx] = 0; uniform_int_distribution<> distribution2(0, xsize - 1);foodx = distribution2(gen);uniform_int_distribution<> distribution3(0, ysize - 1);foody = distribution3(gen);out[(foody * xsize) + foodx] = 2;}
				else {snake.push_back({ snake[snake.size() - 1][0], snake[snake.size() - 1][1] - 1 });}}
			else {
				snake.push_back({ snake[snake.size() - 1][0], snake[snake.size() - 1][1] });
				output = "Username: " + noutput + " Score: 0";
				const char* abb = output.c_str();
				SDL_SetWindowTitle(window, abb);}
			if (ate == false) {snake.erase(snake.begin());}
			bool dup = false;
			vector<vector<int>> snaked = snake;
			sort(snaked.begin(), snaked.end());
			for (int i = 1; i < snaked.size(); i++) { 
				if (snaked[i - 1] == snaked[i]) {
					dup = true;
					break;}}
			if (dup) { snake.clear(); snake.push_back({ xsize / 2, ysize / 2 }); dir = 0; ate = true; out[(foody * xsize) + foodx] = 0;  uniform_int_distribution<> distribution2(0, xsize - 1); foodx = distribution2(gen); uniform_int_distribution<> distribution3(0, ysize - 1); foody = distribution3(gen); out[(foody * xsize) + foodx] = 2;}
			asio::error_code ec;
			asio::io_context context;
			asio::ip::tcp::endpoint endpoint(asio::ip::make_address(ipadress, ec), 6873);
			asio::ip::tcp::socket socket(context);
			socket.connect(endpoint, ec);
			if (socket.is_open()) {
				char c;
				string sRequest = "GET /" + testmsg + "\r\n" "Connection: close\r\n\r\n";
				socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);
				socket.wait(socket.wait_read);
				size_t bytes = socket.available();
				string nout = "";
				if (bytes > 0) {
					vector<char> vBuffer(bytes);
					socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);
					for (int k = 0; k < vBuffer.size(); k++) {
						c = vBuffer[k];
						if (c == '-') { c = ' '; }
						if (k > 197) { nout += c; }}}
				testmsg = "";
				if (snake.size() == 0) { testmsg = "blank"; }
				else {
					for (int ab = 0; ab < snake.size(); ab++) {testmsg += to_string(snake[ab][0]) + "+" + to_string(snake[ab][1]) + "_";}
					testmsg.erase(testmsg.size() - 1, 1);
				}
				others.clear();
				other.clear();
				int counter1 = 0;
				int counter2 = 0;
				others.push_back({ "", "" });
				if (nout.size() != 0) {
					for (int c2 = 0; c2 < nout.size(); c2++) {
						if (nout[c2] == '_') { counter1++; counter2 = 0; others.push_back({ "", "" }); }
						else if (nout[c2] == '+') { counter2++; }
						else {others[counter1][counter2] += nout[c2];}
					}
					for (int c2 = 0; c2 < others.size(); c2++) {other.push_back({ stoi(others[c2][0]),stoi(others[c2][1]) });}
				}
			}
			dup = false;
			vector<vector<int>> others2 = other;
			sort(others2.begin(), others2.end());
			for (int i = 1; i < others2.size(); i++) {
				if (others2[i - 1] == others2[i] && others2[i] == osnake[osnake.size()-1]) {
					dup = true;
					break;
				}
			}
			if (dup) { snake.clear(); snake.push_back({ xsize / 2, ysize / 2 }); dir = 0; ate = true; }
			for (int c2 = 0; c2 < other.size(); c2++) {out[(other[c2][1] * xsize) + other[c2][0]] = 3;}
			out[(foody * xsize) + foodx] = 2;
			for (int c2 = 0; c2 < osnake.size(); c2++) {out[(osnake[c2][1] * xsize) + osnake[c2][0]] = 1;}
			if (snake[snake.size() - 1][1] == foody && snake[snake.size() - 1][0] == foodx) {
				uniform_int_distribution<> distribution2(0, xsize - 1);
				foodx = distribution2(gen);
				uniform_int_distribution<> distribution3(0, ysize - 1);
				foody = distribution3(gen);
				out[(foody * xsize) + foodx] = 2;
				ate = true;
				output = "Username: " + noutput + " Score: " + to_string(snake.size());
				const char* abb = output.c_str();
				SDL_SetWindowTitle(window, abb);
			}
			else {ate = false;}
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			for (int y2 = 0; y2 < ysize; y2++) {
				for (int x2 = 0; x2 < xsize; x2++) {
					SDL_SetRenderDrawColor(renderer, static_cast<int>(round((out[(y2 * xsize) + x2] * 255))) % 257, static_cast<int>(round(out[(y2 * xsize) + x2] * 255)) % 350, static_cast<int>(round(out[(y2 * xsize) + x2] * 255)) % 500, 255);
					if (out[(y2 * xsize) + x2] > 0) { SDL_RenderDrawPoint(renderer, x2, y2); }
				}
			}
			for (int c2 = 0; c2 < other.size(); c2++) {out[(other[c2][1] * xsize) + other[c2][0]] = 0;}
			SDL_RenderPresent(renderer);
		}
		SDL_Delay(delay);
	}
	return 0;
}