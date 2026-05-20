#include "Player.h"
#include "MyEngine/Utils/GlobalVariables.h"

//==========================================
// 初期化
//==========================================
void Player::Initialize() {
    // 調整項目の登録
    RegisterGV();
}

//==========================================
// 更新
//==========================================
void Player::Update() {
    ApplyGV();
    goalState_ = kGoalState;
}

//==========================================
// 描画
//==========================================
void Player::Draw() {

}

//==========================================
// 調整項目の登録・適用
//==========================================
void Player::RegisterGV() {
    auto gv = GlobalVariables::GetInstance();
    auto gN = "Player";
    auto cN = "State";
    gv->AddGroup(gN);
    gv->AddCategory(gN,cN);

    GlobalVariables::ComboItem comboItem;
    comboItem.options = {"Not","Goal"};
    comboItem.currentIndex = 0;
    gv->AddItem<GlobalVariables::ComboItem>(gN, cN, "GoalState", comboItem);
}

void Player::ApplyGV() {
    auto gv = GlobalVariables::GetInstance();
    auto gN = "Player";
    auto cN = "State";
    int index = gv->GetValue<GlobalVariables::ComboItem>(gN, cN, "GoalState").currentIndex;
    kGoalState = (GoalState)index;
}