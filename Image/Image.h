#pragma once
#include "Mode.h"
#include <vector>
#include <iostream>


class Image
{

public:

	Image(); // constructeur par défaut
	Image(int width, int height, int channels, const Mode & mode, unsigned char fillValue);
	Image(int width, int height, int channels, const Mode & mode, const unsigned char* buffer);
	Image(const Image& other); // constructeur de copie

	~Image();


    int getWidth() const;
    int getHeight() const;
    int getChannels() const;
	Mode getMode() const;

    void setWidth(int width);
    void setHeight(int height);
    void setChannels(int channels);
    void setMode(Mode mode);

	unsigned char at(int x, int y, int channel) const;
	unsigned char& at(int x, int y, int channel);

	// Surcharge de l'opérateur d'affectation
	Image& operator=(const Image& other); 
	unsigned char & operator()(int x, int y, int channel);
	const unsigned char & operator()(int x, int y, int channel) const;

	// Opérateurs d'addition
	Image operator+(const Image& other) const;
	Image& operator+=(const Image& other);

	Image operator+(int val)const;
	Image& operator+=(int val);

	Image operator+(const std::vector<int>& pixel) const;
	Image& operator+=(const std::vector<int>& pixel);
	
	//Opérateurs de soustraction

	Image operator-(const Image& other) const;
	Image& operator-=(const Image& other);

	Image operator-(int val)const;
	Image& operator-=(int val);

	Image operator-(const std::vector<int>& pixel) const;
	Image& operator-=(const std::vector<int>& pixel);

	//Operateurs de difference

	Image operator^(const Image& other) const;
	Image& operator^=(const Image& other);

	Image operator^(int val)const;
	Image& operator^=(int val);

	Image operator^(const std::vector<int>& pixel) const;
	Image& operator^=(const std::vector<int>& pixel);
	
	//Opérateurs de multiplication
	Image operator*(float val) const;
	Image& operator*=(float val);

	//Opérateurs de division

	Image operator/(float val) const;
	Image& operator/=(float val);


	// Opérateurs inverse

	Image operator~() const;

	// Opérateurs de comparaison
	Image operator<(int val) const;
	Image operator<=(int val)const;

	Image operator>(int val) const;
	Image operator>=(int val) const;

	Image operator==(int val) const;
	Image operator!=(int val) const;

	friend std::ostream& operator<<(std::ostream& os, const Image& img);

	//charger et sauvegarder une image
	bool save(const std::string& filename) const;
	bool load(const std::string& filename);



private:
	int m_width;
	int m_height;
	int m_channels;
	unsigned char* m_data;
	Mode m_mode;
};
