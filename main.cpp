#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>

#define MAX_BITS 32     // max number of bits allowed per number
#define WINDOW_N 704    // square window dimension

// find largest number smaller than N and divisible by K, used to create the grid
int findNum(int N, int K) {
    int rem = N % K;
    if (rem == 0) return N;
    else return N - rem;
}

// tranform a number inserting 0s in between each bit
int insertZeros(int bits) {
    for (int step = MAX_BITS/2; step >= 1; step = step/2) {
        int aux = bits << step;
        bits |= aux;
        int sub_mask = (1 << step) - 1;
        int mask = 0;

        for (int a = 0; a < MAX_BITS; a = a + step) {
            mask |= sub_mask;
            mask = mask << (step*2);
        }
        mask |= sub_mask;
        bits &= mask;
    }
    return bits;
}

// transform number in gray code back to decimal
int inverseGray(int gray) {
    int binary=0;
    for(;gray;gray=gray>>1) binary^=gray;
    return binary;
}

// transform number to gray code
int gray(int num) {
    return num ^ (num >> 1);
}

// calculate z-order index given a coordinate's x and y positions
int zIndex(int x, int y) {
    x = insertZeros(x);
    y = insertZeros(y);
    y = y << 1;
    return x | y;
}

// calculate gray order index given a coordinate's x and y positions
int gIndex(int x, int y) {
    x = insertZeros(x);
    y = insertZeros(y);
    y = y << 1;
    return inverseGray(x | y);
}

// calculate double gray order index given a coordinate's x and y positions
int dgIndex(int x, int y) {
    x = insertZeros(gray(x));
    y = insertZeros(gray(y));
    y = y << 1;
    return inverseGray(x | y);
}

// struct to store coordinates, each coordinate has x and y positions
// as well as indexes for each order
struct Coord {
    int x;
    int y;
    int z_idx;
    int g_idx;
    int dg_idx;

    Coord(int x, int y) {
        this->x = x;
        this->y = y;
        z_idx = zIndex(x, y);
        g_idx = gIndex(x, y);
        dg_idx = dgIndex(x, y);
    }
};

// clear an image and draw a square grid of grid_n dimensions
void drawGrid(cv::Mat image, int grid_n) {
    image = cv::Scalar(0,0,0);
    int dist = WINDOW_N/grid_n;
    for (int i=0; i<=grid_n; i++) {
        cv::line(image,cv::Point(0,i*dist),cv::Point(findNum(WINDOW_N,grid_n),i*dist),cv::Scalar(255,255,255));
        cv::line(image,cv::Point(i*dist,0),cv::Point(i*dist,findNum(WINDOW_N,grid_n)),cv::Scalar(255,255,255));
    }
}

// get a vector with all the coordinates for a square grid of grid_n dimensions
std::vector<Coord> getListOfCoords(int grid_n) {
    std::vector<Coord> coords_list;
    for (int i = 0; i < grid_n; i++) {
        for (int j = 0; j < grid_n; j++) {
            Coord new_coord(i, j);
            coords_list.push_back(new_coord);
        }
    }
    return coords_list;
}

int main(int argc, char** argv) {
    // default grid dimension
    int grid_n = 8;

    // default list of coordinates
    std::vector<Coord> coords_list = getListOfCoords(grid_n);

    // draw grid and display window
    cv::Mat image(WINDOW_N, WINDOW_N, CV_8UC3, cv::Scalar(0,0,0));
    drawGrid(image, grid_n);
    imshow("Space Filling Curves", image);

    while (true) {
        // wait for keyboard input
        int key = (cv::waitKey(0) & 0xFF);

        switch (key) {
            case 'z': // if z pressed, sort coordinates in z-order
                std::sort(coords_list.begin(), coords_list.end(), [](Coord a, Coord b){return a.z_idx < b.z_idx;}); break;
            case 'x': // if x pressed, sort coordinates in gray order
                std::sort(coords_list.begin(), coords_list.end(), [](Coord a, Coord b){return a.g_idx < b.g_idx;}); break;
            case 'c': // if c pressed, sort coordinates in double gray order
                std::sort(coords_list.begin(), coords_list.end(), [](Coord a, Coord b){return a.dg_idx < b.dg_idx;}); break;
            case 'a': // if a pressed, decrease grid dimension
                if (grid_n>2) grid_n /= 2; break;
            case 's': // if a pressed, increase grid dimension
                grid_n *= 2; break;
            case 'q': // if a pressed, exit
                goto exit_loop;
            default: break;
        }

        // if z, x or c pressed...
        if (key == 'z' || key == 'x' || key == 'c') {
            // clear the grid...
            drawGrid(image, grid_n);
            int dist = WINDOW_N/grid_n;
            cv::Scalar color(0,0,255);
            int thickness = 2;

            // and draw lines in the selected order (sequential traversal since coordinates were previously sorted according to the chosen order)
            for (int i = 0; i < coords_list.size()-1; i++) {
                int new_x1 = coords_list[i].x*dist + dist/2;
                int new_y1 = coords_list[i].y*dist + dist/2;
                int new_x2 = coords_list[i+1].x*dist + dist/2;
                int new_y2 = coords_list[i+1].y*dist + dist/2;
                cv::Point p1(new_x1, new_y1);
                cv::Point p2(new_x2, new_y2);
                cv::line(image, p1, p2, color, thickness);
                cv::putText(image, std::to_string(i), p1, cv::FONT_HERSHEY_PLAIN, 0.8, CV_RGB(0,255,0));
                if (i == coords_list.size()-2) cv::putText(image, std::to_string(i+1), p2, cv::FONT_HERSHEY_PLAIN, 0.8, CV_RGB(0,255,0));
            }
            imshow("Space Filling Curves", image);
        }

        // if a or s pressed...
        else if (key == 'a' || key == 's') {
            // get new list of coordinates (depends on new grid size)...
            coords_list = getListOfCoords(grid_n);

            // and clear grid
            drawGrid(image, grid_n);
            imshow("Space Filling Curves", image);
        }
    }
    
    exit_loop: ;
    return 0;
}

