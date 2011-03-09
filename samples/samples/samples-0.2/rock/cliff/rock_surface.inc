{
  name = rock_surface
  type = MODIFICATOR_POINT_SINGLE

  {
    {
      type = MODIFICATOR_POINT_OPBOX_INIT
      
      color_center_delta.r = rgb(200)
      color_center_delta.g = rgb(200)
      color_center_delta.b = rgb(200)
      
      color_base = color_center_delta*PARAM_RAND;
    }
    
    type = MODIFICATOR_POINT_OPBOX

    color_delta.r = rgb(70)
    color_delta.g = rgb(70)
    color_delta.b = rgb(70)
    
    pixel_color_final = color_base + color_delta*PARAM_RAND
  }
}
