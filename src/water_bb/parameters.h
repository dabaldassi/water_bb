#ifndef PARAMETERS_H
#define PARAMETERS_H

template<const int W, const int H, const int CAT>
class Parameters
{
public:
  
  static constexpr float WIDTH = W;
  static constexpr float HEIGHT = H;
  static constexpr int   CATEGORY = CAT;
};



#endif /* PARAMETERS_H */
