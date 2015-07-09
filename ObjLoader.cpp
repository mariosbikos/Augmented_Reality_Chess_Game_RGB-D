#include "ObjLoader.h"

//Function to use for loading an object
//Parameters:
//----Path:string of path to find the models
//----Out: the vectors of vertices,uvs and normals of specific models read from path file
bool loadOBJ(
	const char * path,
	std::vector < glm::vec3 > & king_vertices,
    std::vector < glm::vec2 > & king_uvs,
    std::vector < glm::vec3 > & king_normals,
	std::vector < glm::vec3 > & queen_vertices,
    std::vector < glm::vec2 > & queen_uvs,
    std::vector < glm::vec3 > & queen_normals,
	std::vector < glm::vec3 > & pawn_vertices,
    std::vector < glm::vec2 > & pawn_uvs,
    std::vector < glm::vec3 > & pawn_normals,
	std::vector < glm::vec3 > & tower_vertices,
    std::vector < glm::vec2 > & tower_uvs,
    std::vector < glm::vec3 > & tower_normals,
	std::vector < glm::vec3 > & knight_vertices,
    std::vector < glm::vec2 > & knight_uvs,
    std::vector < glm::vec3 > & knight_normals,
	std::vector < glm::vec3 > & bishop_vertices,
    std::vector < glm::vec2 > & bishop_uvs,
    std::vector < glm::vec3 > & bishop_normals,
	std::vector< glm::vec3 >  & board_vertices,
	std::vector< glm::vec2 >  & board_uvs,
	std::vector< glm::vec3 >  & board_normals
	)
{
	
	//Declare temporary vectors for indices
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;	// v 1 2 3
	std::vector< glm::vec2 > temp_uvs;		// vt 0.745 0.843
	std::vector< glm::vec3 > temp_normals;	//vn 0.707 0.00 0.707
	char s[15];
	bool start_flag=false;

	FILE * file = fopen(path, "r");
	if( file == NULL )
	{
		printf("Impossible to open the file !\n");
		return false;
	}
	
	while( 1 )
	{
		
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		
		//// else : parse lineHeader
		//IF "o" is found(object data start beneath it),make true the start_flag
		if ( strcmp( lineHeader, "o" ) ==0 && start_flag==false )
		{
			fscanf(file, "%s\n", s );
			cout<<"Read from obj: "<<s<<endl;
			start_flag=true;
		}
		//Whenever new object has been detected-output previous
		else if( (strcmp( lineHeader, "o" ) == 0) && start_flag==true)
		{
			for( unsigned int i=0; i<vertexIndices.size(); i++ )
			{
					unsigned int vertexIndex = vertexIndices[i];
					//there is a -1 because C++ indexing starts at 0 and OBJ indexing starts at 1
					glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
					//one for each object
					if(	strcmp(s,"king")==0 )
						king_vertices.push_back(vertex);	
					else if(strcmp(s,"queen")==0)
						queen_vertices.push_back(vertex);
					else if(strcmp(s,"pawn")==0)
						pawn_vertices.push_back(vertex);
					else if(strcmp(s,"knight")==0)
						knight_vertices.push_back(vertex);
					else if(strcmp(s,"tower")==0)
						tower_vertices.push_back(vertex);
					else if(strcmp(s,"bishop")==0)
						bishop_vertices.push_back(vertex);
					else if(	strcmp(s,"board")==0 )
						board_vertices.push_back(vertex);
					
			}
			

			//for( unsigned int i=0; i<uvIndices.size(); i++ )
			//{
			//		unsigned int uvIndex = uvIndices[i];
			//		//there is a -1 because C++ indexing starts at 0 and OBJ indexing starts at 1
			//		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
			//		out_uvs.push_back(uv);		
			//		//out_uvs[0]= u v of vertex 1 of face 1
			//		//out_uvs[1]= u v of vertex 2 of face 1
			//		//.......//
			//}
			for( unsigned int i=0; i<normalIndices.size(); i++ )
			{
					unsigned int normalIndex = normalIndices[i];
					//there is a -1 because C++ indexing starts at 0 and OBJ indexing starts at 1
					glm::vec3 normals = temp_normals[ normalIndex-1 ];
					if(	strcmp(s,"king")==0 )
						king_normals.push_back(normals);	
					else if(strcmp(s,"queen")==0)
						queen_normals.push_back(normals);
					else if(strcmp(s,"pawn")==0)
						pawn_normals.push_back(normals);
					else if(strcmp(s,"knight")==0)
						knight_normals.push_back(normals);
					else if(strcmp(s,"tower")==0)
						tower_normals.push_back(normals);
					else if(strcmp(s,"bishop")==0)
						bishop_normals.push_back(normals);
					else if(strcmp(s,"board")==0)
						board_normals.push_back(normals);
			}
			*s=0;
			fscanf(file, "%s\n", s );
			cout<<"Read from obj: "<<s<<endl;
			vertexIndices.clear();
			normalIndices.clear();
		}
	

		if ( strcmp( lineHeader, "v" ) == 0 )
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z ); // v 1.000 2.000 3.000
			//Scale down the model, when reading it from file
			vertex.x *= constants::SCALING_FACTOR_MODEL;
			vertex.y *= constants::SCALING_FACTOR_MODEL;
			vertex.z *= constants::SCALING_FACTOR_MODEL;
			temp_vertices.push_back(vertex);		//temp_vertices=vector of 3d coordinates
		}
		else if ( strcmp( lineHeader, "vt" ) == 0 )
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );	// vt 0.745 0.843
			temp_uvs.push_back(uv);
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 )
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );	////vn 0.707 0.00 0.707
			temp_normals.push_back(normal);
		}
		else if ( strcmp( lineHeader, "f" ) == 0 )
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			//int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2] );
			if (matches != 6)
			{
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			/*uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);*/
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
			// For each vertex of each triangle
		
		}
		
	}
	
	for( unsigned int i=0; i<vertexIndices.size(); i++ )
			{
					unsigned int vertexIndex = vertexIndices[i];
					//there is a -1 because C++ indexing starts at 0 and OBJ indexing starts at 1
					glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
					//one for each object
					if(	strcmp(s,"king")==0 )
						king_vertices.push_back(vertex);	
					else if(strcmp(s,"queen")==0)
						queen_vertices.push_back(vertex);
					else if(strcmp(s,"pawn")==0)
						pawn_vertices.push_back(vertex);
					else if(strcmp(s,"knight")==0)
						knight_vertices.push_back(vertex);
					else if(strcmp(s,"tower")==0)
						tower_vertices.push_back(vertex);
					else if(strcmp(s,"bishop")==0)
						bishop_vertices.push_back(vertex);
					else if(	strcmp(s,"board")==0 )
						board_vertices.push_back(vertex);
					
			}
			

			//for( unsigned int i=0; i<uvIndices.size(); i++ )
			//{
			//		unsigned int uvIndex = uvIndices[i];
			//		//there is a -1 because C++ indexing starts at 0 and OBJ indexing starts at 1
			//		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
			//		out_uvs.push_back(uv);		
			//		//out_uvs[0]= u v of vertex 1 of face 1
			//		//out_uvs[1]= u v of vertex 2 of face 1
			//		//.......//
			//}
			for( unsigned int i=0; i<normalIndices.size(); i++ )
			{
					unsigned int normalIndex = normalIndices[i];
					//there is a -1 because C++ indexing starts at 0 and OBJ indexing starts at 1
					glm::vec3 normals = temp_normals[ normalIndex-1 ];
					if(	strcmp(s,"king")==0 )
						king_normals.push_back(normals);	
					else if(strcmp(s,"queen")==0)
						queen_normals.push_back(normals);
					else if(strcmp(s,"pawn")==0)
						pawn_normals.push_back(normals);
					else if(strcmp(s,"knight")==0)
						knight_normals.push_back(normals);
					else if(strcmp(s,"tower")==0)
						tower_normals.push_back(normals);
					else if(strcmp(s,"bishop")==0)
						bishop_normals.push_back(normals);
					else if(strcmp(s,"board")==0)
						board_normals.push_back(normals);
			}
	return true;

}