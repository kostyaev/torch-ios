#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/MSECriterion.c"
#else

static int nn_(MSECriterion_forward)(lua_State *L)
{
  THTensor *input = luaT_checkudata(L, 2, torch_(Tensor_id));  
  THTensor *target = luaT_checkudata(L, 3, torch_(Tensor_id));  
  int sizeAverage = luaT_getfieldcheckboolean(L, 1, "sizeAverage");
  real sum;

  sum = 0;
  TH_TENSOR_APPLY2(real, input, real, target,
                   real z = (*input_data - *target_data);
                   sum += z*z;)

  if(sizeAverage)
    sum /= THTensor_(nElement)(input);

  lua_pushnumber(L, sum);
  lua_setfield(L, 1, "output");

  lua_pushnumber(L, sum);
  return 1;
}

static int nn_(MSECriterion_backward)(lua_State *L)
{
  THTensor *input = luaT_checkudata(L, 2, torch_(Tensor_id));
  THTensor *target = luaT_checkudata(L, 3, torch_(Tensor_id));
  THTensor *gradInput = luaT_checkudata(L, 4, torch_(Tensor_id));
  int sizeAverage = luaT_getfieldcheckboolean(L, 1, "sizeAverage");
  real norm = (sizeAverage ? 2./((real)THTensor_(nElement)(input)) : 2.);

  TH_TENSOR_APPLY3(real, gradInput, real, input, real, target,
                   *gradInput_data = norm * (*input_data - *target_data);)
  return 1;
}

static const struct luaL_Reg nn_(MSECriterion__) [] = {
  {"MSECriterion_forward", nn_(MSECriterion_forward)},
  {"MSECriterion_backward", nn_(MSECriterion_backward)},
  {NULL, NULL}
};

static void nn_(MSECriterion_init)(lua_State *L)
{
  luaT_pushmetaclass(L, torch_(Tensor_id));
  luaT_registeratname(L, nn_(MSECriterion__), "nn");
  lua_pop(L,1);
}

#endif
