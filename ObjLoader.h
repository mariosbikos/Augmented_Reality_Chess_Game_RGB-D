#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "Common.h"
#include "Constants.h"

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
	);

#endif
