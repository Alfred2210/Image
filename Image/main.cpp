#include <iostream>
#include "Image.h"
#include "Mode.h"

int main()
{
    Image img1(10, 10, 1, Mode::GRAY, 100);
    std::cout << "Img1: " << img1 << std::endl;

    std::cout << "\n";

    Image img2(5, 5, 3, Mode::RGB, 10);
    std::cout << "Img2: " << img2 << std::endl;

    std::cout << "Pixel(0,0): " << (int)img1.at(0, 0, 0) << std::endl;
    std::cout << "\n";


    // Test Multiplication
    Image imgMultiplication = img1 * 1.5f;
    std::cout << "Img1 * 1.5: " << (int)imgMultiplication.at(0, 0, 0) << std::endl;
    std::cout << "\n";

    // Test Addition composée
    imgMultiplication += 20;
    std::cout << "Result + 20: " << (int)imgMultiplication.at(0, 0, 0) << std::endl;
    std::cout << "\n";
    // Test Division
    Image imgDiv = imgMultiplication / 2.0f;
    std::cout << "Result / 2: " << (int)imgDiv.at(0, 0, 0) << std::endl;
    std::cout << "\n";

    // Test Inversement
    Image imgInv = ~img1;
    std::cout << "Inverse: " << (int)imgInv.at(0, 0, 0) << std::endl;
    std::cout << "\n";

    // Test Différence
    Image imgDiff = img1 ^ 120;
    std::cout << "Diff (^ 120): " << (int)imgDiff.at(0, 0, 0) << std::endl;
    std::cout << "\n";

	//image + image
    Image imgSum = img1 + img1; // 100 + 100 = 200
    std::cout << "Img1 + Img1: " << (int)imgSum.at(0, 0, 0) << std::endl;
    std::cout << "\n";

    std::vector<int> vec = { 10, 20, 30 };
    Image imgVec = img2 + vec; // Sur l'image RGB
    std::cout << "RGB + Vector: " << (int)imgVec.at(0, 0, 0) << ", "
        << (int)imgVec.at(0, 0, 1) << ", "
        << (int)imgVec.at(0, 0, 2) << std::endl;
    std::cout << "\n";

    //test des operateurs de comparaison

    Image img(2, 2, 1, Mode::GRAY, 100);
    img.at(0, 0, 0) = 50;
    img.at(1, 1, 0) = 200;
    std::cout << "\n";

	
    std::cout << "Image : " << img << std::endl;

    Image test1 = img < 128;
    std::cout << "Test < 128: " << test1 << std::endl;
    std::cout << "Res (50): " << (int)test1.at(0, 0, 0) << std::endl;
    std::cout << "Res (200): " << (int)test1.at(1, 1, 0) << std::endl;

    Image test2 = img <= 50;
    std::cout << "Test <= 50: " << test2 << std::endl;
    std::cout << "Res (50): " << (int)test2.at(0, 0, 0) << std::endl;

    Image test3 = img > 128;
    std::cout << "Test > 128: " << test3 << std::endl;
    std::cout << "Res (200): " << (int)test3.at(1, 1, 0) << std::endl;

    Image test4 = img >= 200;
    std::cout << "Test >= 200: " << test4 << std::endl;
    std::cout << "Res (200): " << (int)test4.at(1, 1, 0) << std::endl;

    Image test5 = img == 100;
    std::cout << "Test == 100: " << test5 << std::endl;
    std::cout << "Res (100): " << (int)test5.at(0, 1, 0) << std::endl;
    std::cout << "Res (50): " << (int)test5.at(0, 0, 0) << std::endl;

    Image test6 = img != 100;
    std::cout << "Test != 100: " << test6 << std::endl;
    std::cout << "Res (100): " << (int)test6.at(0, 1, 0) << std::endl;
    std::cout << "Res (50): " << (int)test6.at(0, 0, 0) << std::endl;
    

    Image original;
    if (original.load("6e4tzycxnzod1.png"))
    {
        std::cout << "Image chargee avec succes : " << original << std::endl;

        
        Image image = original + 150;

        image.save("image_result.png");

        
        Image negative = ~original;
        negative.save("negative_result.jpg");
    }
    else
    {
        std::cerr << "Echec du chargement. Verifiez le chemin" << std::endl;
    }
    
    return 0;
}