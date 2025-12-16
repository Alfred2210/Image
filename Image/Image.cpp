#include "Image.h"
#include <stdexcept>
#include <algorithm>
#include <cmath>

#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


Image::Image()
	: m_width(0), m_height(0), m_channels(0), m_mode(Mode::NONE), m_data(nullptr)
{

}

Image::Image(int width, int height, int channels, const Mode& mode, unsigned char fillValue)
	: m_width(width), m_height(height), m_channels(channels), m_mode(mode)
{

	int size = m_width * m_height * m_channels;

	if (size > 0)
	{
		m_data = new unsigned char[size];
		for (int i = 0; i < size; i++)
		{
			m_data[i] = fillValue;
		}
	}
	else
	{
		m_data = nullptr;
	}

}

Image::Image(int width, int height, int channels, const Mode& mode, const unsigned char* buffer)
	: m_width(width), m_height(height), m_channels(channels), m_mode(mode)
{
	int size = m_width * m_height * m_channels;

	if (size > 0 && buffer != nullptr)
	{
		m_data = new unsigned char[size];
		for (int i = 0; i < size; i++)
		{
			m_data[i] = buffer[i];
		}
	}
	else
	{
		m_data = nullptr;
	}
}

Image::Image(const Image& other)
	: m_width(other.m_width), m_height(other.m_height), m_channels(other.m_channels), m_mode(other.m_mode)
{
	int size = m_width * m_height * m_channels;

	// copie profonde des données
	if (size > 0 && other.m_data != nullptr)
	{
		m_data = new unsigned char[size];

		for (int i = 0; i < size; i++)
		{
			m_data[i] = other.m_data[i];
		}
	}
	else
	{
		m_data = nullptr;
	}
}

Image::~Image()
{
	delete[] m_data;
}

int Image::getWidth() const
{
	return m_width;
}

int Image::getHeight() const
{
	return m_height;
}

int Image::getChannels() const
{
	return m_channels;
}

Mode Image::getMode() const
{
	return m_mode;
}


void Image::setWidth(int width)
{
	m_width = width;
}

void Image::setHeight(int height)
{
	m_height = height;
}

void Image::setChannels(int channels)
{
	m_channels = channels;
}

void Image::setMode(Mode mode)
{
	m_mode = mode;
}

unsigned char Image::at(int x, int y, int channel) const
{
	if ((x < 0 || x >= m_width) || (y < 0 || y >= m_height) || (channel < 0 || channel >= m_channels))
	{
		throw std::out_of_range("Coordonnees en dehors de l'image");
	}
	int index = (y * m_width + x) * m_channels + channel;

	return m_data[index];
}
unsigned char& Image::at(int x, int y, int channel)
{
	if ((x < 0 || x >= m_width) || (y < 0 || y >= m_height) || (channel < 0 || channel >= m_channels))
	{
		throw std::out_of_range("Coordonnees en dehors de l'image");
	}
	int index = (y * m_width + x) * m_channels + channel;

	return m_data[index];
}

Image& Image::operator=(const Image& other)
{
	if (this != &other)
	{
		delete[] m_data;


		m_width = other.m_width;
		m_height = other.m_height;
		m_channels = other.m_channels;
		m_mode = other.m_mode;

		int size = m_width * m_height * m_channels;

		// copie profonde des données
		if (size > 0 && other.m_data != nullptr)
		{
			m_data = new unsigned char[size];
			for (int i = 0; i < size; i++)
			{
				m_data[i] = other.m_data[i];
			}
		}
		else
		{
			m_data = nullptr;
		}
	}

	return *this;
}

unsigned char& Image::operator()(int x, int y, int channel)
{
	return at(x, y, channel);
}

const unsigned char& Image::operator()(int x, int y, int channel) const
{
	return at(x, y, channel);
}

Image Image::operator+(const Image& other) const
{

	if (m_mode != other.m_mode)
	{
		throw std::runtime_error("Erreur : Modes incompatibles");
	}

	int newWidth = std::max(m_width, other.m_width);
	int newHeight = std::max(m_height, other.m_height);

	Image result(newWidth, newHeight, m_channels, m_mode, (unsigned char)0);

	for (int i = 0; i < newWidth; i++)
	{
		for (int j = 0; j < newHeight; j++)
		{
			for (int k = 0; k < m_channels; k++)
			{
				int val = 0;
				// on verifie que les coordonnees sont dans l'image
				if (i < m_width && j < m_height)
				{
					val += this->at(i, j, k);
				}

				int val2 = 0;
				if (i < other.m_width && j < other.m_height)
				{
					val2 += other.at(i, j, k);
				}

				int sum = val + val2;

				if (sum > 255)
				{
					sum = 255;
				}

				result.at(i, j, k) = static_cast<unsigned char>(sum);
			}

		}
	}

	return result;

}

Image& Image::operator+=(const Image& other)
{
	*this = *this + other;

	return *this;
}

Image Image::operator+(int val) const
{
	Image result(m_width, m_height, m_channels, m_mode, (unsigned char)0);


	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			for (int k = 0; k < m_channels; k++)
			{
				int currentVal = this->at(i, j, k);
				int sum = currentVal + val;

				if (sum > 255)
				{
					sum = 255;
				}

				if (sum < 0)
				{
					sum = 0;
				}

				result.at(i, j, k) = static_cast<unsigned char>(sum);
			}

		}
	}

	return result;
}

Image& Image::operator+=(int val)
{
	*this = *this + val;

	return *this;
}

Image Image::operator+(const std::vector<int>& pixel) const
{
	if (pixel.size() != m_channels)
	{
		throw std::runtime_error("Erreur : Taille du pixel incorrecte.");
	}

	Image result(m_width, m_height, m_channels, m_mode, (unsigned char)0);


	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			for (int k = 0; k < m_channels; k++)
			{
				int currentVal = this->at(i, j, k);

				int sum = currentVal + pixel[k];

				if (sum > 255)
				{
					sum = 255;
				}

				if (sum < 0)
				{
					sum = 0;
				}

				result.at(i, j, k) = static_cast<unsigned char>(sum);
			}

		}
	}
	return result;
}

Image& Image::operator+=(const std::vector<int>& pixel)
{
	*this = *this + pixel;
	return *this;
}

Image Image::operator-(const Image& other) const
{
	if (m_mode != other.m_mode)
	{
		throw std::runtime_error("Erreur : Modes ncompatibles");
	}

	int newWidth = std::max(m_width, other.m_width);
	int newHeight = std::max(m_height, other.m_height);

	Image result(newWidth, newHeight, m_channels, m_mode, (unsigned char)0);

	for (int i = 0; i < newWidth; i++)
	{
		for (int j = 0; j < newHeight; j++)
		{
			for (int k = 0; k < m_channels; k++)
			{
				int val = 0;

				// on verifie que les coordonnees sont dans l'image
				if (i < m_width && j < m_height)
				{
					val = this->at(i, j, k);
				}

				int val2 = 0;
				if (i < other.m_width && j < other.m_height)
				{
					val2 = other.at(i, j, k);
				}

				int minus = val - val2;
				if (minus < 0)
				{
					minus = 0;
				}

				if (minus > 255)
				{
					minus = 255;
				}

				result.at(i, j, k) = static_cast<unsigned char>(minus);
			}

		}
	}

	return result;
}

Image& Image::operator-=(const Image& other)
{
	*this = *this - other;
	return *this;
}

Image Image::operator-(int val) const
{
	return (*this) + (-val);
}

Image& Image::operator-=(int val)
{
	*this = *this - val;
	return *this;
}

Image Image::operator-(const std::vector<int>& pixel) const
{
	if (pixel.size() != m_channels)
	{
		throw std::runtime_error("Erreur : Taille du pixel incorrecte.");
	}

	Image result(m_width, m_height, m_channels, m_mode, (unsigned char)0);


	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			for (int k = 0; k < m_channels; k++)
			{
				int currentVal = this->at(i, j, k);

				int minus = currentVal - pixel[k];

				if (minus > 255)
				{
					minus = 255;
				}

				if (minus < 0)
				{
					minus = 0;
				}

				result.at(i, j, k) = static_cast<unsigned char>(minus);
			}

		}
	}
	return result;

}

Image& Image::operator-=(const std::vector<int>& pixel)
{

	*this = *this - pixel;
	return *this;

}

Image Image::operator^(const Image& other) const
{
	if (m_mode != other.m_mode)
	{
		throw std::runtime_error("Erreur : Modes incompatibles");
	}

	int newWidth = std::max(m_width, other.m_width);
	int newHeight = std::max(m_height, other.m_height);

	Image result(newWidth, newHeight, m_channels, m_mode, (unsigned char)0);

	for (int i = 0; i < newWidth; i++)
	{
		for (int j = 0; j < newHeight; j++)
		{
			for (int k = 0; k < m_channels; k++)
			{
				int val = 0;

				// on verifie que les coordonnees sont dans l'image
				if (i < m_width && j < m_height)
				{
					val = this->at(i, j, k);
				}

				int val2 = 0;
				if (i < other.m_width && j < other.m_height)
				{
					val2 = other.at(i, j, k);
				}

				int diff = std::abs(val - val2);


				result.at(i, j, k) = static_cast<unsigned char>(diff);
			}

		}
	}

	return result;
}

Image& Image::operator^=(const Image& other)
{

	*this = *this ^ other;
	return *this;
}

Image Image::operator^(int val) const
{
	Image result(m_width, m_height, m_channels, m_mode, (unsigned char)0);


	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			for (int k = 0; k < m_channels; k++)
			{
				int currentVal = this->at(i, j, k);
				int diff = std::abs(currentVal - val);

				if (diff > 255)
				{
					diff = 255;
				}

				result.at(i, j, k) = static_cast<unsigned char>(diff);
			}

		}
	}

	return result;
}

Image& Image::operator^=(int val)
{

	*this = *this ^ val;
	return *this;
}

Image Image::operator^(const std::vector<int>& pixel) const
{
	if (pixel.size() != m_channels)
	{
		throw std::runtime_error("Erreur : Taille du pixel incorrecte pour la difference absolue.");
	}

	Image result(m_width, m_height, m_channels, m_mode, (unsigned char)0);


	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			for (int k = 0; k < m_channels; k++)
			{
				int currentVal = this->at(i, j, k);

				int diff = std::abs(currentVal - pixel[k]);

				if (diff > 255)
				{
					diff = 255;
				}

				result.at(i, j, k) = static_cast<unsigned char>(diff);
			}

		}
	}
	return result;
}

Image& Image::operator^=(const std::vector<int>& pixel)
{
	*this = *this ^ pixel;
	return *this;
}

Image Image::operator*(float val) const
{
	Image result(m_width, m_height, m_channels, m_mode, (unsigned char)0);

	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			for (int k = 0; k < m_channels; k++)
			{
				int currentVal = this->at(i, j, k);


				int multi = static_cast<int>(currentVal * val);


				if (multi > 255)
				{
					multi = 255;
				}

				if (multi < 0)
				{
					multi = 0;
				}

				result.at(i, j, k) = static_cast<unsigned char>(multi);
			}
		}
	}
	return result;
}

Image& Image::operator*=(float factor)
{
	*this = *this * factor;
	return *this;
}

Image Image::operator/(float val) const
{
	if (val == 0.0f)
	{
		throw std::runtime_error("Erreur : Division par zero.");
	}


	float inverse = 1.0f / val;

	return (*this) * inverse;
}

Image& Image::operator/=(float val)
{
	*this = *this / val;
	return *this;
}

Image Image::operator~() const
{
	Image result(m_width, m_height, m_channels, m_mode, (unsigned char)0);

	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			for (int k = 0; k < m_channels; k++)
			{
				int currentVal = this->at(i, j, k);

				int inverse = 255 - currentVal;

				result.at(i, j, k) = static_cast<unsigned char>(inverse);
			}
		}
	}
	return result;
}

Image Image::operator<(int val) const
{

	Image result(m_width, m_height, 1, Mode::GRAY, (unsigned char)0);

	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{

			int currentVal = this->at(i, j, 0);

			if (currentVal < val)
			{
				result.at(i, j, 0) = 255;
			}
			else
			{
				result.at(i, j, 0) = 0;
			}

		}
	}

	return result;
}

Image Image::operator<=(int val) const
{
	return ~(*this > val);
}

Image Image::operator>(int val) const
{
	Image result(m_width, m_height, 1, Mode::GRAY, (unsigned char)0);

	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			int currentVal = this->at(i, j, 0);

			if (currentVal > val)
			{
				result.at(i, j, 0) = 255;
			}
			else
			{
				result.at(i, j, 0) = 0;
			}
		}
	}

	return result;

}

Image Image::operator>=(int val) const
{
	return ~(*this < val);
}

Image Image::operator==(int val) const
{

	Image result(m_width, m_height, 1, Mode::GRAY, (unsigned char)0);

	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			int currentVal = this->at(i, j, 0);

			if (currentVal == val)
			{
				result.at(i, j, 0) = 255;
			}
			else
			{
				result.at(i, j, 0) = 0;
			}
		}
	}

	return result;
}

Image Image::operator!=(int val) const
{
	return ~(*this == val);
}

std::ostream& operator<<(std::ostream& os, const Image& img)
{

	Mode mode = img.getMode();

	std::string modeStr = "UNKNOWN";

	if (mode == Mode::NONE)
	{
		modeStr = "NONE";
	}
	else if(mode == Mode::RGB)
	{
		modeStr = "RGB";
	}
	else if(mode == Mode::GRAY)
	{
		modeStr = "GRAY";
	}
	os << img.getWidth() << "x" << img.getHeight() << "x" << img.getChannels() << " (" << modeStr << ")";


	return os;
}
bool Image::save(const std::string& filename) const
{
	if (m_data == nullptr || m_width == 0 || m_height == 0) 
	{
		std::cerr << "Erreur: L'image est vide ou n'a pas de dimensions valides." << std::endl;
		return false;
	}

	
	int stride = m_width * m_channels;
	int success = stbi_write_png(filename.c_str(), m_width, m_height, m_channels, m_data, stride);

	if (success) 
	{
		std::cout << "Sauvegarde reussie : " << filename << std::endl;
		return true;
	}
	else 
	{

		std::cerr << "Erreur de sauvegarde pour le fichier : " << filename << std::endl;
		return false;
	}
}

bool Image::load(const std::string& filename)
{
	if (m_data != nullptr) {
		delete[] m_data;
		m_data = nullptr;
	}

	int width, height, channels;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

	if (data == nullptr) 
	{
		std::cerr << "Erreur de chargement de l'image (" << filename << ") : " << stbi_failure_reason() << std::endl;
		return false;
	}
	int size = width * height * channels;
	m_data = new unsigned char[size];

	for (int i = 0; i < size; i++)
	{
		m_data[i] = data[i];
	}


	stbi_image_free(data);
	
	m_width = width;
	m_height = height;
	m_channels = channels;

	
	if (m_channels == 1) {
		m_mode = Mode::GRAY;
	}
	else if (m_channels >= 3) {
		m_mode = Mode::RGB;
	}
	else 
	{
		m_mode = Mode::NONE;
	}

	return true;
}