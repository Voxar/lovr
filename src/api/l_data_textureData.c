#include "api.h"
#include "data/textureData.h"

StringEntry lovrTextureFormat[] = {
  [FORMAT_RGB] = ENTRY("rgb"),
  [FORMAT_RGBA] = ENTRY("rgba"),
  [FORMAT_RGBA4] = ENTRY("rgba4"),
  [FORMAT_R16] = ENTRY("r16"),
  [FORMAT_RG16] = ENTRY("rg16"),
  [FORMAT_RGBA16] = ENTRY("rgba16"),
  [FORMAT_RGBA16F] = ENTRY("rgba16f"),
  [FORMAT_RGBA32F] = ENTRY("rgba32f"),
  [FORMAT_R16F] = ENTRY("r16f"),
  [FORMAT_R32F] = ENTRY("r32f"),
  [FORMAT_RG16F] = ENTRY("rg16f"),
  [FORMAT_RG32F] = ENTRY("rg32f"),
  [FORMAT_RGB5A1] = ENTRY("rgb5a1"),
  [FORMAT_RGB10A2] = ENTRY("rgb10a2"),
  [FORMAT_RG11B10F] = ENTRY("rg11b10f"),
  [FORMAT_D16] = ENTRY("d16"),
  [FORMAT_D32F] = ENTRY("d32f"),
  [FORMAT_D24S8] = ENTRY("d24s8"),
  [FORMAT_DXT1] = ENTRY("dxt1"),
  [FORMAT_DXT3] = ENTRY("dxt3"),
  [FORMAT_DXT5] = ENTRY("dxt5"),
  [FORMAT_ASTC_4x4] = ENTRY("astc4x4"),
  [FORMAT_ASTC_5x4] = ENTRY("astc5x4"),
  [FORMAT_ASTC_5x5] = ENTRY("astc5x5"),
  [FORMAT_ASTC_6x5] = ENTRY("astc6x5"),
  [FORMAT_ASTC_6x6] = ENTRY("astc6x6"),
  [FORMAT_ASTC_8x5] = ENTRY("astc8x5"),
  [FORMAT_ASTC_8x6] = ENTRY("astc8x6"),
  [FORMAT_ASTC_8x8] = ENTRY("astc8x8"),
  [FORMAT_ASTC_10x5] = ENTRY("astc10x5"),
  [FORMAT_ASTC_10x6] = ENTRY("astc10x6"),
  [FORMAT_ASTC_10x8] = ENTRY("astc10x8"),
  [FORMAT_ASTC_10x10] = ENTRY("astc10x10"),
  [FORMAT_ASTC_12x10] = ENTRY("astc12x10"),
  [FORMAT_ASTC_12x12] = ENTRY("astc12x12"),
  { 0 }
};

static int l_lovrTextureDataEncode(lua_State* L) {
  TextureData* textureData = luax_checktype(L, 1, TextureData);
  const char* filename = luaL_checkstring(L, 2);
  bool success = lovrTextureDataEncode(textureData, filename);
  lua_pushboolean(L, success);
  return 1;
}

static int l_lovrTextureDataGetWidth(lua_State* L) {
  TextureData* textureData = luax_checktype(L, 1, TextureData);
  lua_pushinteger(L, textureData->width);
  return 1;
}

static int l_lovrTextureDataGetHeight(lua_State* L) {
  TextureData* textureData = luax_checktype(L, 1, TextureData);
  lua_pushinteger(L, textureData->height);
  return 1;
}

static int l_lovrTextureDataGetDimensions(lua_State* L) {
  TextureData* textureData = luax_checktype(L, 1, TextureData);
  lua_pushinteger(L, textureData->width);
  lua_pushinteger(L, textureData->height);
  return 2;
}

static int l_lovrTextureDataGetFormat(lua_State* L) {
  TextureData* textureData = luax_checktype(L, 1, TextureData);
  luax_pushenum(L, TextureFormat, textureData->format);
  return 1;
}

static int l_lovrTextureDataPaste(lua_State* L) {
  TextureData* textureData = luax_checktype(L, 1, TextureData);
  TextureData* source = luax_checktype(L, 2, TextureData);
  uint32_t dx = luaL_optinteger(L, 3, 0);
  uint32_t dy = luaL_optinteger(L, 4, 0);
  uint32_t sx = luaL_optinteger(L, 5, 0);
  uint32_t sy = luaL_optinteger(L, 6, 0);
  uint32_t w = luaL_optinteger(L, 7, source->width);
  uint32_t h = luaL_optinteger(L, 8, source->height);
  lovrTextureDataPaste(textureData, source, dx, dy, sx, sy, w, h);
  return 0;
}

static int l_lovrTextureDataGetPixel(lua_State* L) {
  TextureData* textureData = luax_checktype(L, 1, TextureData);
  int x = luaL_checkinteger(L, 2);
  int y = luaL_checkinteger(L, 3);
  Color color = lovrTextureDataGetPixel(textureData, x, y);
  lua_pushnumber(L, color.r);
  lua_pushnumber(L, color.g);
  lua_pushnumber(L, color.b);
  lua_pushnumber(L, color.a);
  return 4;
}

static int l_lovrTextureDataSetPixel(lua_State* L) {
  TextureData* textureData = luax_checktype(L, 1, TextureData);
  int x = luaL_checkinteger(L, 2);
  int y = luaL_checkinteger(L, 3);
  Color color = {
    luax_optfloat(L, 4, 1.f),
    luax_optfloat(L, 5, 1.f),
    luax_optfloat(L, 6, 1.f),
    luax_optfloat(L, 7, 1.f)
  };
  lovrTextureDataSetPixel(textureData, x, y, color);
  return 0;
}

static int l_lovrTextureDataGetBlob(lua_State* L) {
  TextureData* textureData = luax_checktype(L, 1, TextureData);
  Blob* blob = textureData->blob;
  luax_pushtype(L, Blob, blob);
  return 1;
}

const luaL_Reg lovrTextureData[] = {
  { "encode", l_lovrTextureDataEncode },
  { "getWidth", l_lovrTextureDataGetWidth },
  { "getHeight", l_lovrTextureDataGetHeight },
  { "getDimensions", l_lovrTextureDataGetDimensions },
  { "getFormat", l_lovrTextureDataGetFormat },
  { "paste", l_lovrTextureDataPaste },
  { "getPixel", l_lovrTextureDataGetPixel },
  { "setPixel", l_lovrTextureDataSetPixel },
  { "getBlob", l_lovrTextureDataGetBlob },
  { NULL, NULL }
};
