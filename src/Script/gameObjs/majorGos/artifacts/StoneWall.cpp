/*
 * ========================= StoneWall.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/majorGos/artifacts/StoneWall.h"

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "Density.h"
#include "animSubspeciesId.h"
#include "dyParams.h"

#include "esrc_shader.h" 
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//


using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs {//------------- namespace gameObjs ----------------


struct StoneWall_PvtBinary{
    int   tmp {};
};



void StoneWall::init(GameObj &goRef_,const DyParam &dyParams_ ){


    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<StoneWall_PvtBinary>();

    //================ dyParams =================//
    size_t typeHash = dyParams_.get_typeHash();
    tprAssert( typeHash == typeid(DyParams_Blueprint).hash_code() );
    const DyParams_Blueprint *bpParamPtr = dyParams_.get_binaryPtr<DyParams_Blueprint>();
    const GoDataForCreate *goDataPtr = bpParamPtr->goDataPtr;
    tprAssert( !goDataPtr->isMultiGoMesh ); // must single gomesh
    const GoDataEntForCreate &goDataEntRef = *(*goDataPtr->goMeshDataUPtrs.cbegin()); // only one

    //----- must before creat_new_goMesh() !!! -----//
    goRef_.actionDirection.reset( goDataPtr->direction );

    if( auto retOpt = goDataPtr->get_brokenLvl(); retOpt.has_value() ){
        goRef_.brokenLvl.reset( retOpt.value() );
    }else{
        tprAssert(0);
    }

    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//
    //-- 制作唯一的 mesh 实例: "root" --
    goRef_.creat_new_goMesh("root", //- gmesh-name
                                goDataEntRef.subspeciesId,
                                AnimActionEName::Idle,
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                goDataEntRef.dposOff, //- pposoff
                                0.0,  //- zOff
                                true //- isVisible
                                );
        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &StoneWall::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &StoneWall::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &StoneWall::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );

    //================ go self vals =================//   
}



void StoneWall::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<StoneWall_PvtBinary>();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_goMesh();
}


void StoneWall::bind( GameObj &goRef_ ){}
void StoneWall::rebind( GameObj &goRef_ ){}
void StoneWall::OnLogicUpdate( GameObj &goRef_ ){}


void StoneWall::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

        cout << "StoneWall::OnActionSwitch" << endl;
        tprAssert(0);
        
}


}//------------- namespace gameObjs: end ----------------

