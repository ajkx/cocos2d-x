
#include "JsonParser.h"

Vector<SpriteData*> JsonParser::getAllSpriteWithFile(std::string fileName){
    // �洢SpriteData���͵ļ���
    Vector<SpriteData*> spriteDataVector;
    // rapidjson����
    rapidjson::Document document;
    // �ؿ�����ʹ��json��Cocos2d-X 3.0���Դ���rapidjson�����ڽ���Json��
    // ���ݴ��ݵĹؿ�ֵ������Ӧ�Ĺؿ������ļ�
    std::string filePath = FileUtils::getInstance()->fullPathForFilename(fileName);
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(filePath);
    document.Parse<0>(contentStr.c_str());
    // ��ȡNSDictionary��ͨ��key��ȡvalue��
    const rapidjson::Value& spriteVector = document["sprites"];
    for(int i = 0;i < spriteVector.Size();i++){
        // ���ÿһ�־����tag��x��y��angle����
        int tag = spriteVector[i]["tag"].GetInt();
        int x = spriteVector[i]["x"].GetInt();
        int y = spriteVector[i]["y"].GetInt();
        // ����SpriteData����洢JSON��Ϣ
        SpriteData* spriteData = SpriteData::create();
        spriteData->setTag(tag);
        spriteData->setX(x);
        spriteData->setY(y);
        // ��SpriteData��ӵ�spriteDataVector����
        spriteDataVector.pushBack(spriteData);
    }
    // ���ش洢SpriteData���͵ļ���
    return spriteDataVector;
}
