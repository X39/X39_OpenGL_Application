#include "Model.h"
#include "globals.h"
#include "CommandHandler\CommandHandler.h"

#include <fstream>
#include <vector>

namespace X39
{
	Model::Model(const char* s)			{ init(std::string(s)); }
	Model::Model(const ::std::string s)	{ init(s); }
	void Model::init(const std::string s)
	{
		path = s;
	}
	Model::~Model(void)
	{

	}


	void Model::loadModelFromDrive(void)
	{

	}

	void Model::loader_dotObj(void)
	{
		char* tempStr;
		double value;
		std::fstream stream;
		char buffer[256];
		std::vector<double> vertexList;
		std::vector<double> textureList;
		std::vector<double> normalsList;
		std::vector<double> vertexIndexList;
		std::vector<double> textureIndexList;
		std::vector<double> normalsIndexList;

		stream.open(path, std::fstream::in);
		if(stream.fail())
		{
			LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(path).append("', cannot read model (.obj) file"));
			stream.close();
			return;
		}

		while(!stream.eof())
		{
			stream.getline(buffer, sizeof(buffer));
			char c1 = buffer[0];
			char c2 = buffer[1];
			char* s;
			switch(c1)
			{
			case '#'://# <string comment> comments
				break;
			case 'm'://mtllib <string Path> link material library
				break;
			case 'o'://o <string Name> Object name
				break;
			case 'v':
				switch(c2)
				{
					case ' '://v <float X> <float Y> <float Z> [ <float W> ] description of a single vertex
						tempStr = buffer;
						value = 0;
						for(int i = 0; i < 3; i++)
						{
							tempStr = strchr(tempStr + 1, ' ');
							if(!::CommandHandler::convAsciiCharToDouble(tempStr, &value))
							{
								LOGGER_WRITE(::Logger::ERRORmsg, std::string("Something moved wrong while parsing obj file! Affected File: ").append(path));
								return;
							}
							vertexList.push_back(value);
						}
						break;
					case 't'://vt <float X> <float Y> description of a single texture vertex.
						tempStr = buffer;
						value = 0;
						for(int i = 0; i < 3; i++)
						{
							tempStr = strchr(tempStr + 1, ' ');
							if(!::CommandHandler::convAsciiCharToDouble(tempStr, &value))
							{
								LOGGER_WRITE(::Logger::ERRORmsg, std::string("Something moved wrong while parsing obj file! Affected File: ").append(path));
								return;
							}
							textureList.push_back(value);
						}
						break;
					case 'n'://vn <float X> <float Y> <float Z> description of a single normal.
						tempStr = buffer;
						value = 0;
						for(int i = 0; i < 3; i++)
						{
							tempStr = strchr(tempStr + 1, ' ');
							if(!::CommandHandler::convAsciiCharToDouble(tempStr, &value))
							{
								LOGGER_WRITE(::Logger::ERRORmsg, std::string("Something moved wrong while parsing obj file! Affected File: ").append(path));
								return;
							}
							normalsList.push_back(value);
						}
						break;
				}
				break;
			case 'g'://g <string groupName> group
				break;
			case 'u'://usemtl <string Path> use material library
				break;
			case 's'://s <integer somestrangeshit> idk the fuck this means, ignore
				break;
			case 'f'://f <integer A_V> / <integer A_VT> / <integer A_VN> <integer B_V> / ... description of an area.
				int slashCount = 0;
				for(int i = strlen(buffer) - 1; i >= 0; i--)
					if(buffer[i] == '/')
						slashCount++;
				if(slashCount % 3 != 0)
				{
					LOGGER_WRITE(::Logger::ERRORmsg, std::string("Cannot read obj files which is not triangulated! Affected File: ").append(path));
					return;
				}
				slashCount /= 3;
				if(slashCount > 2)
				{
					LOGGER_WRITE(::Logger::ERRORmsg, std::string("Cannot read obj file! Affected File: ").append(path));
					return;
				}
				tempStr = buffer;
				for(int i = 0; i < 3; i++)
				{
					tempStr = strchr(tempStr + 1, ' ');
					if(!::CommandHandler::convAsciiCharToDouble(tempStr, &value))
					{
						LOGGER_WRITE(::Logger::ERRORmsg, std::string("Something moved wrong while parsing obj file! Affected File: ").append(path));
						return;
					}
					vertexIndexList.push_back(value);
					if(slashCount > 0)
					{
						s = strchr(tempStr + 1, '/');
						if(slashCount >= 1)
						{
							if(s[1] != '/')
							{
								if(!::CommandHandler::convAsciiCharToDouble(tempStr, &value))
								{
									LOGGER_WRITE(::Logger::ERRORmsg, std::string("Something moved wrong while parsing obj file! Affected File: ").append(path));
									return;
								}
								textureIndexList.push_back(value);
							}
						}
						if(slashCount == 2)
						{
							s = strchr(s + 1, '/');
							if(!::CommandHandler::convAsciiCharToDouble(tempStr, &value))
							{
								LOGGER_WRITE(::Logger::ERRORmsg, std::string("Something moved wrong while parsing obj file! Affected File: ").append(path));
								return;
							}
							textureIndexList.push_back(value);
						}
					}
				}
				break;
			}
		}
	}

	void Model::loadToGpu(void)
	{
	}

	void Model::unloadFromGpu(void)
	{

	}

	void Model::render(void)
	{

	}
};