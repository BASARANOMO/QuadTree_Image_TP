#include <Imagine/Graphics.h>
#include <algorithm>
#include <iostream>
#include "quadtree/quadtree.h"
#include "dfs.h"

using namespace Imagine;
using namespace std;


int main(int argc, char **argv) {
    cout << "************************ RUNNING HORSE SQUARE *********************************" << endl;
    // Get image file (default is running horse)
    const char *image_file =
        (argc > 1) ? argv[1] : srcPath("running-horse-square.png");
    // Load image
    byte* image;
    int width, height;
    cout << "Loading image: " << image_file << endl; 
    loadGreyImage(image_file, image, width, height);
    // Print statistics
    cout << "Image size: " << width << "x" << height << endl;
    cout << "Number of pixels: " << width*height << endl;
    
    QuadTree<byte>* q;
    q = dfs(image, 0, 0, width, width);
    //display(q);
    
    cout << "********************* RUNNING HORSE SQUARE: DAG TREE ***************************" << endl;
    QuadTree<byte>* qDAG;
    qDAG = quaddag(image, 0, 0, width, width, height, true);
    //display(qDAG);
    
    // image decoding
    byte* imageDecoded = new byte[width * height];
    quadtreeDecoding(qDAG, imageDecoded, 0, 0, width, width);
    
    // DAG tree size and compression ratio
    cout << "QuadTree DAG size (bytes in memory): " << sizeQuadTree(qDAG, true) << endl;
    cout << "Compression ratio (nbr leaves / nbr pixels): " << imageCompressionRatio(qDAG, width, height) << endl;
    
    // Display image decoded
    Window window = openWindow(width, height);
    putGreyImage(IntPoint2(0,0), imageDecoded, width, height);
    // Pause
    click();
    
    cout << "************************ RUNNING HORSE RECTANGLE *******************************" << endl;
    // Get image file (default is running horse)
    const char *image_file2 =
        (argc > 1) ? argv[1] : srcPath("running-horse-rect.png");
    // Load image
    byte* image2;
    int width2, height2;
    cout << "Loading image: " << image_file2 << endl;
    loadGreyImage(image_file2, image2, width2, height2);
    // Print statistics
    cout << "Image size: " << width2 << "x" << height2 << endl;
    cout << "Number of pixels: " << width2*height2 << endl;
    int length2 = nextPow2(max(width2, height2));

    QuadTree<byte>* qDAG2;
    qDAG2 = quaddag(image2, 0, 0, length2, width2, height2, true);
    
    // image decoding
    byte* imageDecoded2 = new byte[length2 * length2];
    quadtreeDecoding(qDAG2, imageDecoded2, 0, 0, length2, length2);
    
    // Display image decoded
    //Window window2 = openWindow(length2, length2);
    putGreyImage(IntPoint2(0,0), imageDecoded2, length2, length2);
    // Pause
    click();

    cout << "************************ LOSSY COMPRESSION *******************************" << endl;
    // Get image file (default is running horse)
    const char *image_file3 =
        (argc > 1) ? argv[1] : srcPath("lossy_compression.png");
    // Load image
    byte* image3;
    int width3, height3;
    cout << "Loading image: " << image_file3 << endl;
    loadGreyImage(image_file3, image3, width3, height3);
    // Print statistics
    cout << "Image size: " << width3 << "x" << height3 << endl;
    cout << "Number of pixels: " << width3*height3 << endl;
    
    QuadTree<byte>* q3;
    q3 = dfsLossyCompression(image3, 0, 0, width, width, height, true, byte(50));
    //display(q3);
    
    // image decoding
    byte* imageDecoded3 = new byte[width3 * height3];
    quadtreeDecoding(q3, imageDecoded3, 0, 0, width3, height3);
    
    // Display image decoded
    putGreyImage(IntPoint2(0,0), imageDecoded3, width3, height3);
    // Pause
    click();
    
    // delete pointers
    delete image;
    delete image2;
    delete image3;
    
    delete q;
    delete qDAG;
    delete qDAG2;
    delete q3;
    
    delete[] imageDecoded;
    delete[] imageDecoded2;
    delete[] imageDecoded3;

    delete black;
    delete white;
    // Exit
    return 0;
}
