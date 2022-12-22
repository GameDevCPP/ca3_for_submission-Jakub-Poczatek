#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include "bmp.h"

using namespace std;

MenuScene menu;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;

int main() {
    Engine::Start(1280, 720, "CA3",&menu);
    /*GridWithWeights grid = make_diagram4();
    GridLocation start{1, 4}, goal{8, 3};
    std::unordered_map<GridLocation, GridLocation> came_from;
    std::unordered_map<GridLocation, double> cost_so_far;
    a_star_search(grid, start, goal, came_from, cost_so_far);
    //draw_grid(grid, nullptr, &came_from, nullptr, &start, &goal);
    std::cout << '\n';
    std::vector<GridLocation> path = reconstruct_path(start, goal, came_from);
    draw_grid(grid, nullptr, nullptr, &path, &start, &goal);
    std::cout << '\n';
    draw_grid(grid, &cost_so_far, nullptr, nullptr, &start, &goal);
     */
    /*
    BMP bmp = BMP("../../res/Level1.bmp");
    auto pixels = bmp.getColouredPixels();
    for (int i = 0; i < pixels.size(); i++){
        cout << i % bmp.getWidth() << ", " << i / bmp.getWidth() << ": {" << to_string(pixels[i].r) << ", " << to_string(pixels[i].g) << ", " << to_string(pixels[i].b) << "}" << endl;
    }*/
}