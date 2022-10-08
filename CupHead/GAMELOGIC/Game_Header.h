#pragma once

#define Floor -300.f
#define Set_Y_Floor TRANS()->LPOS({TRANS()->LPOS().X, Floor + (m_Collision->LSCALE().Y / 2), TRANS()->LPOS().Z})
#define PI 3.141592f