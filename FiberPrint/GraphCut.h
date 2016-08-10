/*
* ==========================================================================
*
*		Class:	GraphCut
*
* Description:	Cut input model into layers by different means.
*
*	  Version:  1.1
*	  Created:  Oct/10/2015
*     Updated:  Mar/25/2016
*
*	   Author:  Xin Hu, Yijiang Huang, Guoxian Song
*	  Company:  GCL@USTC
*
*	Successor:	ADMMCut - FrameFab ADMM cut
*				NormalCut - cut layers by sweeping from bottom to top.
*				NoneCut - no layers
* ==========================================================================
*/

#pragma once

#include <iostream>

#include "WireFrame\WireFrame.h"
#include "Stiffness\Stiffness.h"
#include "Collision\QuadricCollision.h"
#include "FiberPrintPARM.h"

using namespace std;


class GraphCut
{
public:
	GraphCut();
	virtual ~GraphCut();

public:
	virtual void	MakeLayers();	
	virtual void	PrintOutTimer();

public:
	WireFrame		*ptr_frame_;
	char			*ptr_path_;


	vector<int>		cutting_edge_;


protected:
	/* for debuging */
	bool			debug_;

};

