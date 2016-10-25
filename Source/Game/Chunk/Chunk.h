#ifndef CHUNK_H
#define CHUNK_H

#include <unordered_map>
#include <memory>
#include <vector>
#include <mutex>

#include <SFML/Graphics.hpp>

#include "OpenGL/Glm/glm_transformations.h"
#include "Chunk_Location.h"
#include "Model.h"
#include "Entity.h"
#include "AABB.h"

#include "Blocks.h"


class Texture_Atlas;
class World;

class Chunk;
typedef std::unique_ptr<Chunk> Chunk_Ptr;

class Chunk
{
    struct Chunk_Part
    {
        Model model;
        std::vector<GLfloat> vertexData;
        std::vector<GLfloat> textureData;

        void buffer ();
    };

    public:
        Chunk( std::unordered_map<Chunk_Location, Chunk_Ptr>& chunkMap,
               const Chunk_Location& location,
               const Texture_Atlas& atlas,
               World& world  );

        void generateBlockData      ();

        bool hasBlockData   () const;
        bool hasVertexData  () const;
        bool hasBuffered    () const;

        void generateMesh   ();
        void bufferMesh     ();

        const Block_t& getBlock ( int x, int y, int z ) const;
        const Block_t& getBlock ( const Vector3& location ) const;

        const AABB getBlockAABBTop ( const Vector3& location ) const;

        const Chunk_Location& getLocation () const;
        const Vector2&  getPosition () const;

        void setBlock   (   GLuint x, GLuint y, GLuint z, Block::ID id, bool overrideBlocks = true );

        const Model& getChunkModel  () const;
        const Model& getWaterModel  () const;
        const Model& getFloraModel  () const;

        void setToDelete        ();
        bool shouldBeDeleted    ();

    private:
        void generateStructureData  ();

        void makeBlock ( GLfloat x, GLfloat y, GLfloat z, const Block_t& block );

        void makeBack   (   GLfloat x, GLfloat y, GLfloat z, const Block_t& block );
        void makeTop    (   GLfloat x, GLfloat y, GLfloat z, const Block_t& block );
        void makeLeft   (   GLfloat x, GLfloat y, GLfloat z, const Block_t& block );
        void makeRight  (   GLfloat x, GLfloat y, GLfloat z, const Block_t& block );
        void makeFront  (   GLfloat x, GLfloat y, GLfloat z, const Block_t& block );
        void makeBottom (   GLfloat x, GLfloat y, GLfloat z, const Block_t& block );

        void finalizeFace ( const std::vector<GLfloat> verticies,
                            const std::vector<GLfloat> textureCoords,
                            Chunk_Part& part );

        void makeTree   (   GLuint x, GLuint y, GLuint z );

        void setBlock   (   GLuint x, GLuint y, GLuint z, Block_t& block, bool overrideBlocks = true );

        const Block_t& getAdjChunkBlock ( int xChange, int zChange, int blockX, int blockY, int blockZ ) const;

        void genAdjChunks( const Chunk_Location& location );

    private:
        std::unordered_map<Chunk_Location, Chunk_Ptr>* m_p_chunkMap;
        std::vector<Block_t*> m_blocks;

        std::vector<Vector3> m_treeLocations;

        Chunk_Location m_location; //Map coords
        Vector2  m_position; //GL coords

        const Texture_Atlas* m_p_atlas;

        World* m_p_world;

        bool m_hasBlockData     = false;
        bool m_hasVertexData    = false;
        bool m_hasBufferedData  = false;
        bool m_shouldBeDeleted  = false;

        Chunk_Part m_solidPart;
        Chunk_Part m_waterPart;
        Chunk_Part& getPart ( const Block_t& block );

    public:
        static constexpr int SIZE  = 24,
                             HEIGHT = 200,
                             WATER_LEVEL = 115,
                             BEACH_LEVEL = WATER_LEVEL + 2;
};


#endif // CHUNK_H
