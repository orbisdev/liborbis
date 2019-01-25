/*****************************************************************************
 * ==> MiniModels -----------------------------------------------------------*
 *****************************************************************************
 * Description : This module provides the functions to generate models like  *
 *               characters, landscapes, ...                                 *
 * Developer   : Jean-Milost Reymond                                         *
 * Copyright   : 2015 - 2018, this file is part of the Minimal API. You are  *
 *               free to copy or redistribute this file, modify it, or use   *
 *               it for your own projects, commercial or not. This file is   *
 *               provided "as is", without ANY WARRANTY OF ANY KIND          *
 *****************************************************************************/

#ifndef MiniModelsH
#define MiniModelsH

// std
#include <stdio.h>

// mini API
#include "MiniCommon.h"
#include "MiniGeometry.h"
#include "MiniVertex.h"

//----------------------------------------------------------------------------
// Global defines
//----------------------------------------------------------------------------

#define M_MDL_Mesh_File_Version 6
#define M_MDL_ID                (('O' << 24) + ('P' << 16) + ('D' << 8) + 'I')

#define M_MD2_Mesh_File_Version 8
#define M_MD2_ID                (('2' << 24) + ('P' << 16) + ('D' << 8) + 'I')

//----------------------------------------------------------------------------
// Structures
//----------------------------------------------------------------------------

/**
* Model mesh
*/
typedef struct
{
    float*       m_pVertexBuffer;
    unsigned int m_VertexCount;
    int          m_IsTriangleStrip;
} MINI_Mesh;

/**
* Model frame
*/
typedef struct
{
    MINI_Mesh*   m_pMesh;
    unsigned int m_MeshCount;
} MINI_Frame;

/**
* Model OpenGL commands
*/
typedef struct
{
    int m_GLCmd;    // 0 = TRIANGLE_STRIP, 1 = TRIANGLE_FAN
    int m_CullMode; // 0 = DISABLED,       1 = ENABLED
} MINI_MdlCmds;

/**
* MDL header
*/
typedef struct
{
    unsigned int m_ID;
    unsigned int m_Version;
    float        m_Scale[3];
    float        m_Translate[3];
    float        m_BoundingRadius;
    float        m_EyePosition[3];
    unsigned int m_SkinCount;
    unsigned int m_SkinWidth;
    unsigned int m_SkinHeight;
    unsigned int m_VertexCount;
    unsigned int m_PolygonCount;
    unsigned int m_FrameCount;
    unsigned int m_SyncType;
    unsigned int m_Flags;
    float        m_Size;
} MINI_MDLHeader;

/**
* MDL skin
*/
typedef struct
{
    unsigned int   m_Group;
    unsigned int   m_Count;
    unsigned int   m_TexLen;
    float*         m_pTime;
    unsigned char* m_pData;
} MINI_MDLSkin;

/**
* MDL texture coordinate
*/
typedef struct
{
    unsigned int m_OnSeam;
    unsigned int m_U;
    unsigned int m_V;
} MINI_MDLTextureCoord;

/**
* MDL polygon
*/
typedef struct
{
    unsigned int m_FacesFront;
    unsigned int m_VertexIndex[3];
} MINI_MDLPolygon;

/**
* MDL vertex
*/
typedef struct
{
    unsigned char m_Vertex[3];
    unsigned char m_NormalIndex;
} MINI_MDLVertex;

/**
* MDL frame
*/
typedef struct
{
    MINI_MDLVertex  m_BoundingBoxMin;
    MINI_MDLVertex  m_BoundingBoxMax;
    char            m_Name[16];
    MINI_MDLVertex* m_pVertex;
} MINI_MDLFrame;

/**
* MDL frame group
*/
typedef struct
{
    unsigned char  m_Type;
    unsigned char  m_Count;
    MINI_MDLVertex m_Min;
    MINI_MDLVertex m_Max;
    float*         m_pTime;
    MINI_MDLFrame* m_pFrame;
} MINI_MDLFrameGroup;

/**
* MDL model
*/
typedef struct
{
    MINI_VertexFormat* m_pVertexFormat;
    MINI_Frame*        m_pFrame;
    unsigned int       m_FrameCount;
} MINI_MDLModel;

/**
* MD2 header
*/
typedef struct
{
    unsigned int m_ID;
    unsigned int m_Version;
    unsigned int m_SkinWidth;
    unsigned int m_SkinHeight;
    unsigned int m_FrameSize;
    unsigned int m_SkinCount;
    unsigned int m_VertexCount;
    unsigned int m_TextureCoordCount;
    unsigned int m_PolygonCount;
    unsigned int m_GlCmdsCount;
    unsigned int m_FrameCount;
    unsigned int m_SkinOffset;
    unsigned int m_TextureCoordOffset;
    unsigned int m_PolygonOffset;
    unsigned int m_FrameOffset;
    unsigned int m_GlCmdsOffset;
    unsigned int m_EndOffset;
} MINI_MD2Header;

/**
* MD2 model skin
*/
typedef struct
{
    char m_Name[64];
} MINI_MD2Skin;

/**
* MD2 vertex
*/
typedef struct
{
    unsigned char m_Vertex[3];
    unsigned char m_NormalIndex;
} MINI_MD2Vertex;

/**
* MD2 texture coordinates
*/
typedef struct
{
    unsigned short m_U;
    unsigned short m_V;
} MINI_MD2TextureCoord;

/**
* MD2 frame
*/
typedef struct
{
    char            m_Name[16];
    float           m_Scale[3];
    float           m_Translate[3];
    MINI_MD2Vertex* m_pVertex;
} MINI_MD2Frame;

/**
* MD2 polygon
*/
typedef struct
{
    unsigned short m_VertexIndex[3];
    unsigned short m_TextureCoordIndex[3];
} MINI_MD2Polygon;

/**
* MD2 model
*/
typedef struct
{
    MINI_VertexFormat* m_pVertexFormat;
    MINI_Frame*        m_pFrame;
    unsigned int       m_FrameCount;
} MINI_MD2Model;

#ifdef __cplusplus
    extern "C"
    {
#endif

        //----------------------------------------------------------------------------
        // MDL functions
        //----------------------------------------------------------------------------

        /**
        * Reads MDL header
        *@param pFile - MDL file
        *@param[out] pHeader - MDL header structure to populate
        */
        void miniReadMDLHeader(FILE* pFile, MINI_MDLHeader* pHeader);

        /**
        * Reads MDL skin
        *@param pFile - MDL file
        *@param[out] pSkin - MDL skin structure to populate
        */
        void miniReadMDLSkin(FILE* pFile, const MINI_MDLHeader* pHeader, MINI_MDLSkin* pSkin);

        /**
        * Reads MDL texture coordinates
        *@param pFile - MDL file
        *@param[out] pTexCoord - MDL texture coordinates structure to populate
        */
        void miniReadMDLTextureCoord(FILE* pFile, MINI_MDLTextureCoord* pTexCoord);

        /**
        * Reads MDL polygon
        *@param pFile - MDL file
        *@param[out] pPolygon - MDL polygon structure to populate
        */
        void miniReadMDLPolygon(FILE* pFile, MINI_MDLPolygon* pPolygon);

        /**
        * Reads MDL vertex
        *@param pFile - MDL file
        *@param[out] pVertex - MDL vertex structure to populate
        */
        void miniReadMDLVertex(FILE* pFile, MINI_MDLVertex* pVertex);

        /**
        * Reads MDL frame
        *@param pFile - MDL file
        *@param pHeader - MDL header
        *@param[out] pFrame - MDL frame structure to populate
        */
        void miniReadMDLFrame(FILE* pFile, const MINI_MDLHeader* pHeader, MINI_MDLFrame* pFrame);

        /**
        * Reads MDL frame group
        *@param pFile - MDL file
        *@param pHeader - MDL header
        *@param[out] pFrameGroup - MDL frame group structure to populate
        */
        void miniReadMDLFrameGroup(      FILE*               pFile,
                                   const MINI_MDLHeader*     pHeader,
                                         MINI_MDLFrameGroup* pFrameGroup);

        /**
        * Uncompresses the texture
        *@param pSkin - model skin
        *@param index - texture index
        *@param[out] pTexture - texture
        */
        void miniUncompressMDLTexture(const MINI_MDLSkin* pSkin,
                                            unsigned int  index,
                                            MINI_Texture* pTexture);

        /**
        * Uncompresses MDL vertex
        *@param pHeader - MDL header
        *@param pVertex - MDL vertex
        *@param[out] pResult - resulting vector
        */
        void miniUncompressMDLVertex(const MINI_MDLHeader* pHeader,
                                     const MINI_MDLVertex* pVertex,
                                           MINI_Vector3*   pResult);

        /**
        * Creates mdl mesh
        *@param pHeader - MDL header
        *@param pFrameGroups - MDL frame groups
        *@param pSkin - MDL skin
        *@param pTexCoord - MDL texture coordinates
        *@param pPolygon - MDL polygon
        *@param pVertexFormat - vertex format to use
        *@param color - color in RGBA format
        *@param[out] pMDLModel - MDL model
        *@return 1 on success, otherwise 0
        */
        int miniCreateMDLMesh(MINI_MDLHeader*       pHeader,
                              MINI_MDLFrameGroup*   pFrameGroups,
                              MINI_MDLSkin*         pSkin,
                              MINI_MDLTextureCoord* pTexCoord,
                              MINI_MDLPolygon*      pPolygon,
                              MINI_VertexFormat*    pVertexFormat,
                              unsigned              color,
                              MINI_MDLModel**       pMDLModel);

        /**
        * Loads MDL model
        *@param pName - MDL file name
        *@param pVertexFormat - vertex format to use
        *@param color - color in RGBA format
        *@param[out] pMDLModel - MDL model
        *@param[out] pTexture - MDL texture
        *@return 1 on success, otherwise 0
        */
        int miniLoadMDLModel(const unsigned char*     pName,
                                   MINI_VertexFormat* pVertexFormat,
                                   unsigned           color,
                                   MINI_MDLModel**    pMDLModel,
                                   MINI_Texture*      pTexture);

        /**
        * Releases MDL model
        *@param pMDLModel - MDL model to release
        */
        void miniReleaseMDLModel(MINI_MDLModel* pMDLModel);

        //----------------------------------------------------------------------------
        // MD2 functions
        //----------------------------------------------------------------------------

        /**
        * Reads MD2 header
        *@param pFile - MD2 file
        *@param[out] pHeader - MD2 header structure to populate
        */
        void miniReadMD2Header(FILE* pFile, MINI_MD2Header* pHeader);

        /**
        * Reads MD2 skin
        *@param pFile - MD2 file
        *@param[out] pSkin - MD2 skin structure to populate
        */
        void miniReadMD2Skin(FILE* pFile, MINI_MD2Skin* pSkin);

        /**
        * Reads MD2 vertex
        *@param pFile - MD2 file
        *@param[out] pVertex - MD2 vertex structure to populate
        */
        void miniReadMD2Vertex(FILE* pFile, MINI_MD2Vertex* pVertex);

        /**
        * Reads MD2 texture coordinates
        *@param pVertex - MD2 file
        *@param[out] pTexCoord - MD2 texture coordinates structure to populate
        */
        void miniReadMD2TextureCoord(FILE* pFile, MINI_MD2TextureCoord* pTexCoord);

        /**
        * Reads MD2 frame
        *@param pFile - MD2 file
        *@param pHeader - MD2 header
        *@param[out] pFrame - MD2 frame structure to populate
        */
        void miniReadMD2Frame(FILE* pFile, const MINI_MD2Header* pHeader, MINI_MD2Frame* pFrame);

        /**
        * Reads MD2 polygon
        *@param pFile - MD2 file
        *@param[out] pPolygon - MD2 polygon  structure to populate
        */
        void miniReadMD2Polygon(FILE* pFile, MINI_MD2Polygon* pPolygon);

        /**
        * Uncompresses MD2 vertex
        *@param pFrame - MD2 frame
        *@param pVertex - MD2 vertex
        *@param[out] pResult - resulting vector
        */
        void miniUncompressMD2Vertex(const MINI_MD2Frame*  pFrame,
                                     const MINI_MD2Vertex* pVertex,
                                           MINI_Vector3*   pResult);

        /**
        * Creates md2 mesh
        *@param pHeader - MD2 header
        *@param pFrames - MD2 frames
        *@param pSkin - MD2 skin
        *@param pTexCoord - MD2 texture coordinates
        *@param pPolygon - MD2 polygon
        *@param pGlCmds - MD2 OpenGL commands
        *@param pVertexFormat - vertex format to use
        *@param color - color in RGBA format
        *@param[out] pMD2Model - MD2 model
        *@return 1 on success, otherwise 0
        */
        int miniCreateMD2Mesh(MINI_MD2Header*       pHeader,
                              MINI_MD2Frame*        pFrames,
                              MINI_MD2Skin*         pSkin,
                              MINI_MD2TextureCoord* pTexCoord,
                              MINI_MD2Polygon*      pPolygon,
                              int*                  pGlCmds,
                              MINI_VertexFormat*    pVertexFormat,
                              unsigned              color,
                              MINI_MD2Model**       pMD2Model);

        /**
        * Loads MD2 model
        *@param pName - MD2 file name
        *@param pVertexFormat - vertex format to use
        *@param color - color in RGBA format
        *@param[out] pMD2Model - MD2 model
        *@return 1 on success, otherwise 0
        */
        int miniLoadMD2Model(const unsigned char*     pName,
                                   MINI_VertexFormat* pVertexFormat,
                                   unsigned           color,
                                   MINI_MD2Model**    pMD2Model);

        /**
        * Releases MD2 model
        *@param pMD2Model - MD2 model to release
        */
        void miniReleaseMD2Model(MINI_MD2Model* pMD2Model);

        //----------------------------------------------------------------------------
        // Landscape creation functions
        //----------------------------------------------------------------------------

        /**
        * Loads data to generate landscape from model image
        *@param pFileName - bitmap file name to load from
        *@param[out] pData - data to use to generate landscape
        *@param[out] width - landscape width, in bytes
        *@param[out] height - landscape height, in bytes
        *@return 1 on success, otherwise 0
        *@note For now only most common bitmaps are loaded, some bitmap types may be unsupported
        */
        int miniLoadLandscape(const unsigned char*  pFileName,
                                    unsigned char** pData,
                                    int*            pWidth,
                                    int*            pHeight);

        /**
        * Generates a landscape from a grayscale image
        *@param pData - image data
        *@param mapX - map width, same as image width in pixels
        *@param mapZ - map deep, same as image height in pixels
        *@param height - map height
        *@param scale - scale factor
        *@param[out] pMesh - mesh containing landscape
        *@param[out] pMeshSize - size of landscape mesh
        */
        void miniGenerateLandscapeMesh(unsigned char* pData,
                                       int            mapX,
                                       int            mapZ,
                                       float          height,
                                       float          scale,
                                       MINI_Vector3** pMesh,
                                       unsigned int*  pMeshSize);

        /**
        * Creates a landscape mesh from a grayscale image
        *@param pData - data image to use to generate landscape
        *@param mapX - map width, same as image width in pixels
        *@param mapZ - map deep, same as image height in pixels
        *@param height - map height
        *@param scale - scale factor
        *@param pVertexFormat - landscape mesh vertex format
        *@param color - landscape mesh color
        *p@aram[out] pMesh - mesh containing landscape
        */
        void miniCreateLandscape(unsigned char*     pData,
                                 int                mapX,
                                 int                mapZ,
                                 float              height,
                                 float              scale,
                                 MINI_VertexFormat* pVertexFormat,
                                 unsigned int       color,
                                 MINI_Mesh**        pMesh);
        
        /**
        * Releases the landscape
        *@param pLandscape - landscape to release
        */
        void miniReleaseLandscape(MINI_Mesh* pLandscape);

        //----------------------------------------------------------------------------
        // Rover creation functions
        //----------------------------------------------------------------------------

        /**
        * Creates a rover model
        *@param[in, out] pVertexFormat - vertex format, generated format on function ends
        *@param[out] pVertices - model vertices, should be deleted whenever useless
        *@param[out] pVertexCount - vertex count
        *@param[out] pMdlCmds - model commands, should be deleted whenever useless
        *@param[out] pIndexes - index buffer, should be deleted whenever useless
        *@param[out] pIndexCount - index count
        */
        void miniCreateRover(MINI_VertexFormat* pVertexFormat,
                             float**            pVertices,
                             unsigned*          pVertexCount,
                             MINI_MdlCmds**     pMdlCmds,
                             MINI_Index**       pIndexes,
                             unsigned*          pIndexCount);

#ifdef __cplusplus
    }
#endif

//----------------------------------------------------------------------------
// Compiler
//----------------------------------------------------------------------------

// needed in mobile c compiler to link the .h file with the .c
#if defined(_OS_IOS_) || defined(_OS_ANDROID_) || defined(_OS_WINDOWS_)
    #include "MiniModels.c"
#endif

#endif // MiniModelsH
