#include "GameScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

static int monsterCount = 5; // 每一波出现多少怪物
static int _currentLevel = 1;  // 当前关卡

Scene* Game::createSceneWithLevel(int selectLevel)
{
    // 获得关卡数
    _currentLevel = selectLevel;
    
    // 创建一个场景对象，该对象将会由自动释放池管理内存的释放
    auto scene = Scene::create();
    
    // 创建层对象，该对象将会由自动释放池管理内存的释放
    auto layer = Game::create();
    
    // 将Game层作为子节点添加到场景
    scene->addChild(layer);
    
    // 返回场景对象
    return scene;
}

bool Game::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    // 获得设备支持的可见OpenGL视图大小（即窗口大小）。
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _screenWidth = visibleSize.width;
    _screenHeight = visibleSize.height;
	_isFinish = false;
	_isSelect = false;
    
    /*************读取关卡数据开始**********************/
    // rapidjson对象
    rapidjson::Document document;
    // 关卡数据使用json，Cocos2d-X 3.0中自带了rapidjson，用于解析Json。
    // 根据传递的关卡值解析对应的关卡数据文件
    std::string filePath = FileUtils::getInstance()->fullPathForFilename(StringUtils::format("%d.data",_currentLevel));
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(filePath);
    document.Parse<0>(contentStr.c_str());
    
    // 读取地图文件名称
    _tileFile = document["tileFile"].GetString();
    // 读取怪物波数
    _number = document["number"].GetInt();
    // 出现怪物取模基数
    _delivery = 55;
    // 当前出现怪物数量
    _currentCount = 0;
    // 当前怪物波数
    _currNum = 1;
    // 初始化金币数量
    _goldValue = 200;

    // 调用TMXTiledMap的函数读取TileMap.tmx文件创建一个瓦片地图对象
    _tileMap = TMXTiledMap::create(_tileFile);
    // 把地图的锚点和位置都设置为原点，这样可以使地图的左下角与屏幕的左下角对齐
    _tileMap->setAnchorPoint(Vec2::ZERO);
    _tileMap->setPosition(Vec2::ZERO);
    // 将地图添加为当前层的子节点
    this->addChild(_tileMap,1);
    
    // 获取障碍层，并设置障碍层为不可见
    _collidable = _tileMap->getLayer("collidable");
    _collidable->setVisible(false);
    
    // 获得关卡设定的怪物
    const rapidjson::Value& monsterArray = document["monsters"];
    for(unsigned int i = 0;i < monsterArray.Size();i++){
        // 获得每一种怪物的name、lifeValue和gold数据
        std::string name = monsterArray[i]["name"].GetString();
        int lifeValue = monsterArray[i]["lifeValue"].GetInt();
        int gold = monsterArray[i]["gold"].GetInt();
        // 将获得的name、lifeValue和gold数据存储到自定义MonsterData对象当中
        auto monsterData = MonsterData::create();
        monsterData->setName(name);
        monsterData->setLifeValue(lifeValue);
        monsterData->setGold(gold);
        // 将MonsterData存储到关卡怪物数据的集合当中
        _monsterDatas.pushBack(monsterData);
    }
    // 获得关卡设定的炮塔
    const rapidjson::Value& turretArray = document["turrets"];
    for(unsigned int i = 0;i < turretArray.Size();i++){
        // 获得每一种怪物的name、gold和bulletName数据
        std::string name = turretArray[i]["name"].GetString();
        int gold = turretArray[i]["gold"].GetInt();
        std::string bulletName = turretArray[i]["bulletName"].GetString();
        // 将获得的name、gold和bulletName数据存储到自定义TurretData对象当中
        auto turretData = TurretData::create();
        turretData->setName(name);
        turretData->setGold(gold);
        turretData->setBulletName(bulletName);
        // 将TurretData存储到关卡炮塔数据的集合当中
        _turretDatas.pushBack(turretData);
    }
    // 获得怪物经过的路径
    const rapidjson::Value& pathArray = document["path"];
    for(unsigned int i = 0;i < pathArray.Size();i++){
        // 获得每个怪物经过路径的x和y值
        int x = pathArray[i]["x"].GetInt();
        int y = pathArray[i]["y"].GetInt();
        // 根据x和y值创建一个坐标，该坐标是一个地图坐标
        Vec2 tilePoint = Vec2(x, y);
        // 将地图坐标转成屏幕坐标
        Vec2 locationPoint = locationForTilePos(tilePoint);
        // 由于Point不继承Ref，Vector不能存储，所以设计了PointDelegate类代理存储数据
        auto pointDelegate = PointDelegate::create(locationPoint.x, locationPoint.y);
        // 将每一个屏幕坐标存储到路径集合当中
        _pathPoints.pushBack(pointDelegate);
    }
    // 获得名称为princessObject的对象层
    auto princessObject = _tileMap->getObjectGroup("princessObject");
    // 获得名称为princess的地图对象，该对象在地图的位置就是公主出现的位置
    ValueMap princessValueMap = princessObject->getObject("princess");
    int princessX = princessValueMap.at("x").asInt();
    int princessY = princessValueMap.at("y").asInt();
    // 创建公主，并将地图中对象的x和y值作为公主出现的位置
    _princess = Sprite::createWithSpriteFrameName("princess.png");
    _princess->setPosition(princessX, princessY);
    // 将公主添加为地图的子节点
    _tileMap->addChild(_princess,2);

	/*************设置屏幕上方显示的游戏数据开始**********************/
    
    // 显示出现了多少波怪物
    _currNumLabel = Label::createWithSystemFont("0", "Arial", 32);
    _currNumLabel->setColor(Color3B::RED);
    _currNumLabel->setPosition(_screenWidth*0.45, _screenHeight*0.96);
    _tileMap->addChild(_currNumLabel,2);
    // 共多少波怪物
    _numberLabel = Label::createWithSystemFont(StringUtils::format("/%d波怪物",_number), "Arial", 32);
    _numberLabel->setColor(Color3B::BLUE);
    _numberLabel->setPosition(_screenWidth*0.55, _screenHeight*0.96);
    _tileMap->addChild(_numberLabel,2);
    
    // 左上角玩家金币数量
    auto gold = Sprite::createWithSpriteFrameName("gold.png");
    gold->setPosition(50, _screenHeight*0.96);
    _tileMap->addChild(gold,2);
    
    _goldLabel = Label::createWithSystemFont("200", "Arial", 32);
    _goldLabel->setColor(Color3B::RED);
    _goldLabel->setPosition(100, _screenHeight*0.96);
    _tileMap->addChild(_goldLabel,2);
    
	// 开始游戏，倒数3下
    Label* label1 = Label::createWithSystemFont("1", "Arial-BoldMT", 150);
    Label* label2 = Label::createWithSystemFont("2", "Arial-BoldMT", 150);
    Label* label3 = Label::createWithSystemFont("3", "Arial-BoldMT", 150);
    label1->setColor(Color3B::BLUE);
    label2->setColor(Color3B::BLUE);
    label3->setColor(Color3B::BLUE);
    label1->setPosition(_screenWidth/2,_screenHeight/2);
    label2->setPosition(_screenWidth/2,_screenHeight/2);
    label3->setPosition(_screenWidth/2,_screenHeight/2);
    // 设置Label不显示
    label1->setVisible(false);
    label2->setVisible(false);
    label3->setVisible(false);
    _tileMap->addChild(label1,2);
    _tileMap->addChild(label2,2);
    _tileMap->addChild(label3,2);
    // 完成倒数的Sequence动作，3，2，1，每隔1秒显示1个数字，同时删除之前显示的数字
    auto countdown = Sequence::create(
                                      CallFunc::create([=]{
        label3->setVisible(true);
    }),DelayTime::create(1),CallFunc::create([=]{
        _tileMap->removeChild(label3);
    }),CallFunc::create([=]{
        label2->setVisible(true);
    }),DelayTime::create(1),CallFunc::create([=]{
        _tileMap->removeChild(label2);
    }),CallFunc::create([=]{
        label1->setVisible(true);
    }),DelayTime::create(1),CallFunc::create([=]{
        _tileMap->removeChild(label1);
        // 游戏主循环开始
        scheduleUpdate();
    }), NULL);
    // 执行倒数动作
    this->runAction(countdown);
    
    // 播放背景音乐
    SimpleAudioEngine::getInstance()->playBackgroundMusic(MUSIC_FILE,true);
    // 设置背景音乐大小
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);

	 // 预先准备好可建造的炮塔 add.png
    auto addImage=Sprite::create("add.png");
    addImage->setScale(0.75);
    addImage->setVisible(false);
    _tileMap->addChild(addImage,1);
    // 将可建造炮塔添加为addImage的子节点
    for ( int i=0; i<_turretDatas.size(); i++) {
        // 循环炮塔数据的集合获取每一个炮塔
        auto data=_turretDatas.at(i);
        // 创建炮塔
        auto turret=Turret::create(data->getName());
        turret->setGold(data->getGold());
        turret->setScale(1.8f);
        turret->setPosition(60*i, addImage->getContentSize().height*1.5+10);
        // 将炮塔添加为addImage的子节点
        addImage->addChild(turret);
    }
    
    // 创建事件监听器
    auto gameListener = EventListenerTouchOneByOne::create();
    // 响应触摸事件函数
    gameListener->onTouchBegan=[=](Touch* touch, Event* event){
        // OpenGL坐标
        Vec2 touchLocation = touch->getLocation();
        // 获得addImage所有子节点（即炮塔）
        Vector<Node*> turrets=addImage->getChildren();
        for ( int i=0; i<turrets.size(); i++) {
            // 获得每一个炮塔
            auto node=turrets.at(i);
            Rect rect=Rect(0, 0, node->getContentSize().width, node->getContentSize().height);
            Turret* turret=(Turret*)node;
             // 如果addImage显示，说明玩家是第二次触摸，选择建造炮塔
            if (addImage->isVisible()) {
               // 如果不够金币建造炮塔，返回false
                if (turret->getGold() > _goldValue) {
                    _isSelect=false;
                    addImage->setVisible(false);
                    return false;
                }else{
                    // 如果可以建造炮塔，设置_isSelect=true
                    if (rect.containsPoint(node->convertToNodeSpace(touchLocation))) {
                        addImage->setTag(i);
                        _isSelect=true;
                        return true;
                    }
                }
            }
            // 玩家是第一次触摸
            else
            {
                // 如果建造炮塔金币不够，设置炮塔透明度，表示不可建造
                if (turret->getGold() > _goldValue) {
                    turret->setOpacity(100);
                }
            }
        }
        return true;
    };
    gameListener->onTouchEnded = [=](Touch *touch, Event *event){
        // OpenGL坐标
        Vec2 touchLocation = touch->getLocation();
        // 相对Node的坐标
        Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
        // 同一个位置上不能放置炮塔
        for (int i = 0; i < _turretVector.size(); i++) {
            auto temp = _turretVector.at(i);
            if (temp->getBoundingBox().containsPoint(nodeLocation)) {
                return ;
            }
        }
        // _isSelect为true，用户是选择建造炮塔，false，用户选择建造炮塔的位置
        if (_isSelect) {
            _isSelect=false;
            // 创建一个炮塔对象
            TurretData *data=_turretDatas.at(addImage->getTag());
            Turret *turret=Turret::create(data->getName());
            turret->setScale(1.8f);
            // 将炮塔设置在addImage的位置上
            turret->setPosition(addImage->getPosition());
            turret->setBulletName(data->getBulletName());
            _tileMap->addChild(turret,1);
            // 隐藏addImage
            addImage->setVisible(false);
            // 将炮塔添加到炮塔集合
            _turretVector.pushBack(turret);
            // 消费金币
            _goldValue-=data->getGold();
        }else{
            // 如果准备建造炮塔的位置不是障碍物时
            if (!this->getCollidable(nodeLocation)) {
                // addImage显示
                addImage->setVisible(true);
                // 设置addImage位置为触碰位置
                addImage->setPosition(nodeLocation);
            }
        }
    };
    
    // 添加场景优先事件监听器
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(gameListener, this);
    
    
	return true;
}

/**
 * 获得动画动作函数
 * animName：动画帧的名字
 * delay：动画帧与帧之间的间隔时间
 * animNum：动画帧的数量
 */
Animate* Game::getAnimateByName(std::string animName,float delay,int animNum){
    // 创建动画
    Animation* animation = Animation::create();
    // 循环从精灵帧缓存中获取与图片名称相对应的精灵帧组成动画
	for(unsigned int i = 1;i<=animNum;i++){
        // 获取动画图片名称，例如pest1.png
        std::string frameName = animName;
        frameName.append(StringUtils::format("%d.png",i));
		// 将单张图片添加为精灵帧（即动画帧）
		animation->addSpriteFrameWithFile(frameName.c_str());
    }
    // 设置动画播放的属性
	animation->setDelayPerUnit(delay);
	// 让精灵对象在动画执行完后恢复到最初状态
	animation->setRestoreOriginalFrame(true);
    // 返回动画动作对象
    Animate* animate = Animate::create(animation);
    return animate;
}

// TileMap坐标转换为OpenGL坐标
Vec2 Game::locationForTilePos(Vec2 pos){
    int x = (int)(pos.x*(_tileMap->getTileSize().width/CC_CONTENT_SCALE_FACTOR()));
	float pointHeight = _tileMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	int y = (int)((_tileMap->getMapSize().height * pointHeight) - (pos.y * pointHeight));
	return Vec2(x,y);
}

// 将OpenGL坐标转换为TileMap坐标
Vec2 Game::tileCoordForPosition(Vec2 position){
    // CC_CONTENT_SCALE_FACTOR 在iPhone上视网膜显示返回2。否则返回1
	// 玩家位置的x除以地图的宽，得到的是地图横向的第几个格子（tile）
    int x = (int)(position.x / (_tileMap->getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
    // 玩家位置的y除以地图的高，得到的是地图纵向第几个格子（tile），
	// 因为Cocos2d-x的y轴和TileMap的y轴相反，所以使用地图的高度减去玩家位置的y
    float pointHeight = _tileMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
    int y = (int)((_tileMap->getMapSize().height * pointHeight - position.y) / pointHeight);
    return Vec2(x,y);
}

void Game::update(float delta){
    _count++;
	updateMonster(delta);
    detectionTurret(delta);
    collisionDetection(delta);
    detectionPrincess(delta);
    updateHUD(delta);
}

void Game::updateMonster(float delta){
    // _isFinish标记为false表示可以出现怪物
    if (!_isFinish ) {
        // _delivery变量为取模基数，通过设置该变量值，可以控制怪物出现频率
        if (_count % _delivery == 0) {
            // 当前出现怪物数量加1
            _currentCount++;
            // 如果当前出现怪物数量大于每一波出现多少怪物，表示一波结束
            if (_currentCount > monsterCount) {
                // 关卡怪物总波数减1
                _number -= 1;
                // 当关卡怪物总波数为0
                if (_number == 0) {
                    // 设置标记，表示关卡所有怪物全部出现完毕
                    _isFinish = true;
                }else{
                    // 重新开始一波怪物
                    _currentCount = 0; // 当前出现怪物数量归零
                    _delivery -= 5; // 改变取模基数，则怪物出现速度加快
                    _currNum += 1; // 当前出现怪物波数加1
                }
            }else{
                // 调用TMXTiledMap的getObjectGroup函数获取对象层
                auto pestObject = _tileMap->getObjectGroup("monsterObject");
                // 根据对象名称获取对象的信息
                ValueMap pestValueMap = pestObject->getObject("monster");
                // 获取地图中设置的player对象的x和y值
                int pestX = pestValueMap.at("x").asInt();
                int pestY = pestValueMap.at("y").asInt();
                /**
                 * 一个关卡可能出现多种怪物，可以在json文件中设置
                 * 随机创建怪物精灵，并将地图中对象的x和y值作为怪物出现的位置
                 */
				int random = CCRANDOM_0_1()* _monsterDatas.size();
                auto monsterData = _monsterDatas.at(random);
                // 根据怪物名称创建怪物
                auto monster = Monster::create(monsterData->getName());
                
                // 设置怪物名称、生命值、消灭怪物的金币值和怪物出现坐标
                monster->setName(monsterData->getName());
                monster->setLifeValue(monsterData->getLifeValue());
                monster->setGold(monsterData->getGold());
                monster->setScale(0.7f);
                monster->setPosition(pestX, pestY);
                
                // 创建进度条
                auto loadingBar = LoadingBar::create("planeHP.png");
                // 设置进度条方向
                loadingBar->setDirection(LoadingBar::Direction::LEFT);
                // 设置进度条大小
                loadingBar->setScale(0.06f, 0.1f);
                // 设置进度条百分比
                loadingBar->setPercent(100);
                // 将进度条设置为怪物血条
                monster->setHP(loadingBar);
                // 添加怪物血条
                _tileMap->addChild(monster->getHP(),2);
                // 计算血条更新量
                monster->setHPInterval(100/monster->getLifeValue());
                // 将怪物添为地图子节点
                _tileMap->addChild(monster,2);
                // 将怪物添加到已出现怪物数组
                _monsterVector.pushBack(monster);
                // 怪物根据路径点移动
                monsterMoveWithWayPoints(_pathPoints, monster);
            }
        }
    }else{
        // _isFinish标记为true表示关卡所有怪物已经出现完毕
        if (_monsterVector.size() <= 0) {
            // 游戏胜利
            gameOver(1);
        }
    }
}


/**
 * 怪物根据路径点移动
 * pathVector：从关卡json文件中获取的路径集合
 * monster：移动的怪物
 */
void Game::monsterMoveWithWayPoints(Vector<PointDelegate*> pathVector, Monster* monster){
    // 定义动作集合，用来保存多个moveTo动作
    Vector<FiniteTimeAction*> actionVector;
    // 循环路径点集合，创建多个MoveTo动作，玩家将执行多个MoveTo动作完成移动
    for (int i = 0; i < pathVector.size(); i++) {
        // 获得需要移动的每一个点
        PointDelegate* pd = pathVector.at(i);
        Vec2 glPoint = Vec2(pd->getX(),pd->getY());
        // 创建MoveTo一个动作，让精灵对象移动到指定的位置
        MoveTo* moveTo = MoveTo::create(0.2f, glPoint);
        // 将MoveTo动作添加到临时数组
        actionVector.pushBack(moveTo);
    }
    // 截取怪物名称
    std::string monsterName = monster->getName();
    size_t iPos = monsterName.find(".png");
    monsterName = monsterName.substr(0, iPos);
    // 调用getAnimateByName函数获得玩家精灵移动动画
    auto animate = getAnimateByName(monsterName, 0.5f, 5);
    // 创建一个动作，重复执行Animate动画
    auto repeatanimate = RepeatForever::create(animate);
    // 玩家精灵重复执行动画动作
    monster->runAction(repeatanimate);
    // 创建回调动作，当MoveTo动作完成后精灵恢复最初站立状态
    auto callfunc = CallFunc::create([=]{
        // 停止动画
        monster->stopAction(repeatanimate);
    });
    actionVector.pushBack(callfunc);
    // 按顺序执行动作集合中的动作
    auto sequence = Sequence::create(actionVector);
    // 执行一系列的动作
    monster->runAction(sequence);
}

void Game::updateHUD(float delta){
    // 更新金币数量
    _goldLabel->setString(StringUtils::format("%d",_goldValue));
    // 更新怪物波数量
    _currNumLabel->setString(StringUtils::format("%d",_currNum));
    // 设置每一个怪物血条位置，血条跟随怪物移动
    for ( int i = 0; i < _monsterVector.size(); i++) {
        auto monster = _monsterVector.at(i);
        // 如果怪物不为nullptr，并且怪物生命值不为0的时候
        if (monster != nullptr && monster->getLifeValue() > 0) {
            // 怪物精灵坐标
            Vec2 pos = monster->getPosition();
            // 设置怪物血条显示的坐标位置
            monster->getHP()->setPosition(Vec2(pos.x, pos.y+40));
        }
    }
}


/**
 * 本游戏采用圆形的攻击范围，直接计算怪物所在的坐标点到炮塔所在的坐标点的距离少于攻击半径即可。
 * 检测一点是否在圆内部
 * monsterPoint 怪物所在的坐标点
 * turretPoint 炮塔所在的坐标点
 * area 攻击范围
 */
bool Game::checkPointInCircle(Vec2 monsterPoint, Vec2 turretPoint, int area)
{
    int x = monsterPoint.x - turretPoint.x;
    int y = monsterPoint.y - turretPoint.y;
    // sqrt函数求平方根
    if (sqrt(x * x + y * y) <= area) return true;
    return false;
}

/**
 * 发射炮弹时计算炮弹的旋转方向
 * monsterPoint 怪物所在的坐标点
 * turret 炮塔
 */
float Game::getTurretRotation(Vec2 monsterPoint,Turret* turret){
    // 求旋转的角度，需要使用三角代数：反正切 = 对面 / 邻边
    // monsterPoint.x：怪物坐标的x轴，turret->getPosition().x：炮塔坐标的x轴
    int offX = monsterPoint.x - turret->getPosition().x;
    // monsterPoint.y：怪物坐标的y轴，turret->getPosition().y：炮塔坐标的y轴
    int offY = monsterPoint.y - turret->getPosition().y;
    // 旋转弧度 = atan2f(反正切函数)（对面/邻边）
    float radian = atan2f(offY,offX);
    // CC_RADIANS_TO_DEGREES函数可以将弧度转化为角度
    float degrees = CC_RADIANS_TO_DEGREES(radian);
    // 转出来的角度和本例炮弹图片相差90度，因此，为了得到正确的方向，我们把需要将结果进行转换。
    return 90 - degrees;
}

// 根据炮弹移动距离计算炮弹需要移动时间
float Game::getBulletMoveTime(Vec2 start, Vec2 end){
    // 将起点和终点的坐标转换为TileMap坐标
    Vec2 tileStart = tileCoordForPosition(start);
    Vec2 tileEnd = tileCoordForPosition(end);
    // 移动一个网格的时间
    float duration = 0.1f;
    // 根据移动网格计算移动时间
    duration = duration * sqrtf((tileStart.x - tileEnd.x) * (tileStart.x - tileEnd.x)
                                + (tileStart.y - tileEnd.y) * (tileStart.y - tileEnd.y));
    return duration;
}


bool Game::getCollidable(Vec2 position)
{
    // 将屏幕坐标转成地图坐标，用于判断是否可以放置炮塔
    Vec2 tilePos = tileCoordForPosition(position);
    // 如果触摸点是不可放置炮塔（即有障碍物）的位置，则直接return
    int tileGid = _collidable->getTileGIDAt(tilePos);
    if (tileGid) {
        // 使用GID来查找指定tile的属性，返回一个Value
        Value properties = _tileMap->getPropertiesForGID(tileGid);
        // 返回的Value实际是一个ValueMap
        ValueMap map = properties.asValueMap();
        // 查找ValueMap，判断是否有障碍物，如果有，直接返回
        std::string value = map.at("collidable").asString();
        if (value.compare("true") == 0) {
            return true;
        }
    }
    return false;
}

// 检测炮塔
void Game:: detectionTurret(float delta){
    // 遍历炮塔集合
    for ( int i = 0; i < _turretVector.size(); i++) {
        // 获得每一个炮塔
        auto turret = _turretVector.at(i);
        // 游戏设定炮塔一次只能发射一枚炮弹，如果炮塔没有关联炮弹或者炮弹已经发射完成
        if (turret->getBullet() == nullptr || !turret->getBullet()->isShoot()) {
            // 遍历怪物集合
            for ( int j = 0; j < _monsterVector.size(); j++) {
                auto monster = _monsterVector.at(j);
                // 检测怪物是否在炮塔的攻击范围
                bool flag = checkPointInCircle(monster->getPosition(),turret->getPosition(),200);
                if(flag){
                    // 创建炮弹
                    auto bullet = Bullet::createWithSpriteFrameName(turret->getBulletName());
                    bullet->setScale(0.8f);
                    bullet->setPosition(turret->getPosition().x, turret->getPosition().y);
                    _tileMap->addChild(bullet,2);
                    // 获得需要旋转的角度
                    float cocosAngle = getTurretRotation(monster->getPosition(), turret);
                    // 根据炮弹发射方向旋转炮塔
                    turret->runAction(RotateTo::create(0.05f, cocosAngle));
                    // 计算炮弹移动的时间，避免因为距离长短而造成炮弹运行的速度问题
                    float duration = getBulletMoveTime(bullet->getPosition(),monster->getPosition());
                    // 炮弹移动动作
                    auto moveTo = MoveTo::create(duration, monster->getPosition());
                    bullet->runAction(moveTo);
                    // 将炮弹添加到炮弹数组
                    _bulletVector.pushBack(bullet);
                    // 设置炮弹发射标记
                    bullet->setShoot(true);
                    //  设置炮塔和炮弹关联
                    turret->setBullet(bullet);
                    break;
                }
            }
            
        }
    }
}


// 检测炮弹和怪物的碰撞
void Game::collisionDetection(float delta){
    // 遍历怪物数组
    for (unsigned int i =0; i < _monsterVector.size(); i++) {
        Monster* monster = _monsterVector.at(i);
        // 遍历炮弹数组
        for (unsigned int j = 0; j < _bulletVector.size(); j++) {
            auto bullet = _bulletVector.at(j);
            // 如果怪物与炮弹发生了碰撞
            if(monster->getBoundingBox().intersectsRect(bullet->getBoundingBox())){
                // 设置怪物的生命值减1
                monster->setLifeValue(monster->getLifeValue()-1);
                // 更新怪物血条
                if (monster->getHP()!=nullptr) {
                    monster->getHP()->setPercent(monster->getHpInterval()*monster->getLifeValue());
                }
                // 如果怪物生命值为0
                if (monster->getLifeValue() <= 0) {
                    // 播放怪物被消灭的音效
                    SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE1);
                    // 从怪物集合中删除怪物
                    _monsterVector.eraseObject(monster);
                    // 从地图上删除怪物
                    _tileMap->removeChild(monster);
                    // 金币增加
                    _goldValue += monster->getGold();
                }
                // 从炮弹集合中删除炮弹
                _bulletVector.eraseObject(bullet);
                // 从地图上删除炮弹
                _tileMap->removeChild(bullet);
                // 修改炮弹发射标记
                bullet->setShoot(false);
                break;
            }else{
                // 如果炮弹到达攻击坐标，而怪物已经走开，即炮弹没有打中怪物
                if(bullet->getNumberOfRunningActions() == 0){
                    // 删除炮弹
                    _bulletVector.eraseObject(bullet);
                    // 从地图上删除炮弹
                    _tileMap->removeChild(bullet);
                    // 修改炮弹发射标记
                    bullet->setShoot(false);
                }
            }
        }
    }
}

// 检测公主
void Game::detectionPrincess(float delta){
    // 遍历每一个怪物
    for (unsigned int j = 0; j < _monsterVector.size(); j++) {
        auto monster = _monsterVector.at(j);
        // 如果怪物与攻击公主，游戏失败
        if(monster->getBoundingBox().containsPoint(_princess->getPosition())){
            // 游戏失败
            gameOver(0);
        }
    }
}

// 游戏结束
void Game::gameOver(int tag){
    
    // 停止游戏主循环
    this->unscheduleUpdate();
    // 停止所有怪物动作
    for (unsigned int i = 0; i < _monsterVector.size(); i++) {
        auto monster = _monsterVector.at(i);
        monster->stopAllActions();
    }
    // 删除屏幕所有炮弹
    for (unsigned int i = 0; i < _bulletVector.size(); i++) {
        auto bullet = _bulletVector.at(i);
        bullet->stopAllActions();
        _tileMap->removeChild(bullet);
    }
    _bulletVector.clear();
    
    // 选择屏幕块
    auto temp = Sprite::createWithSpriteFrameName("block.png");
    temp->setPosition(_screenWidth/2, _screenHeight/2);
    temp->setColor(Color3B::GRAY);
    _tileMap->addChild(temp,2);
    
    // 停止背景音乐
    SimpleAudioEngine::getInstance()->stopBackgroundMusic(MUSIC_FILE);
    
    // 你共击退了10波怪物!
    auto messageLabel = Label::createWithSystemFont(StringUtils::format("你共击退了[%d]波怪物!",_currNum), "Arial-BoldMT", 38);
    messageLabel->setColor(Color3B::WHITE);
    messageLabel->setPosition(_screenWidth/2, _screenHeight/2+50);
    _tileMap->addChild(messageLabel,4);
    
    // “选择关卡”按钮
    auto select_button = Button::create("btn-select.png");
    select_button->setPosition(Vec2(_screenWidth/2-100, _screenHeight/2-80));
    select_button->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            // 右边滑入剧场效果
            auto transition = TransitionSlideInR::create(2.0, LevelManager::createScene());
            // push游戏场景
            Director::getInstance()->pushScene(transition);
        }
    });
    this->addChild(select_button,3);

    // “再试一次”或"进入下一关"按钮
    auto play_button = Button::create();
    if(tag == 1){
        // 进入下一关
        play_button->loadTextures("btn-next.png", "btn-next.png");
        _currentLevel++;
    }
    else{
        // 再试一次
        // 播放游戏失败音效
        SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE2);
        play_button->loadTextures("btn-again.png", "btn-again.png");
    }
    play_button->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            // TransitionSplitRows 左右滑动剧场效果
            auto transition = TransitionSplitRows::create(2.0, Game::createSceneWithLevel(_currentLevel));
            // push游戏场景
            Director::getInstance()->pushScene(transition);
        }
    });
    play_button->setPosition(Vec2(_screenWidth/2+100, _screenHeight/2-80));
    this->addChild(play_button,3);
    
}