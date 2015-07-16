
#include "JsonParser.h"

Vector<SpriteData*> JsonParser::getAllSpriteWithFile(std::string fileName){
    // 存储SpriteData类型的集合
    Vector<SpriteData*> spriteDataVector;
    // rapidjson对象
    rapidjson::Document document;
    // 关卡数据使用json，Cocos2d-X 3.0中自带了rapidjson，用于解析Json。
    // 根据传递的关卡值解析对应的关卡数据文件
    std::string filePath = FileUtils::getInstance()->fullPathForFilename(fileName);
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(filePath);
    document.Parse<0>(contentStr.c_str());
    // 读取NSDictionary，通过key获取value，
    const rapidjson::Value& spriteVector = document["sprites"];
    for(int i = 0;i < spriteVector.Size();i++){
        // 获得每一种精灵的tag、x、y和angle数据
        int tag = spriteVector[i]["tag"].GetInt();
        int x = spriteVector[i]["x"].GetInt();
        int y = spriteVector[i]["y"].GetInt();
        // 创建SpriteData对象存储JSON信息
        SpriteData* spriteData = SpriteData::create();
        spriteData->setTag(tag);
        spriteData->setX(x);
        spriteData->setY(y);
        // 将SpriteData添加到spriteDataVector数组
        spriteDataVector.pushBack(spriteData);
    }
    // 返回存储SpriteData类型的集合
    return spriteDataVector;
}
