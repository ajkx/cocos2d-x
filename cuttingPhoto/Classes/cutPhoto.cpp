#include "cutPhoto.h"

cuttingPhotos* cuttingPhotos::create(const char* photo,unsigned int rows,unsigned int columns)
{
    cuttingPhotos *cp=new cuttingPhotos();
    
    if (cp && cp->initWithPhoto(photo,rows,columns)) {
        cp->autorelease();
        return cp;
    }
    
    CC_SAFE_DELETE(cp);
    
    return nullptr;
}


bool cuttingPhotos::initWithPhoto(const char* photo,unsigned int rows,unsigned int columns)
{
    bool result;//��ʼ���Ƿ�ɹ�
    
    if (Layer::init()) {
        _rows=rows;
        _columns=columns; 
        if (initSlice(photo)) {
            
            initTouchEventlistener();
            
            restart();

            result=true;
        }else{
            result=false;
        }
        
    }else{
        result =false;
    }
    return result;
}

bool cuttingPhotos::initSlice(const char* photo)
{
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(photo);
    
    if (texture==nullptr) {
        return false;
    }
    
    //��ʼ���洢���е��б�
    for (int w=0; w<_rows; ++w) {
        std::vector<Sprite*> row;
        for (int h=0; h<_columns; ++h) {
            row.push_back(nullptr);
        }
        
        _slices.push_back(row);
    }
    
    //����ͼƬÿ�еĿ��
    auto size=texture->getContentSize();
    _sliceWidth=size.width/_rows;
    _sliceHeight=size.height/_columns;
    
    this->setContentSize(size);//����Layer��С
    
    //���һ��
    auto endSlice=Sprite::createWithTexture(texture,Rect(_sliceWidth*(_rows-1),_sliceHeight*(_columns-1),_sliceWidth,_sliceHeight));
    endSlice->setAnchorPoint(Vec2(0,0));
    endSlice->setPosition((_rows-1)*_sliceWidth,0);
    endSlice->setOpacity(100);//����͸����
    this->addChild(endSlice);
	//��Ϊ�գ���ʾ����������
	_slices[_rows-1][_columns-1] = nullptr;

	//������Ƭ
	for(int w = 0; w < _rows;w++){
		for(int h = 0; h < _columns; h++){
			if(w == _rows - 1 && h == _columns - 1){
				break;
			}
			Sprite* slice = Sprite::createWithTexture(texture,Rect(_sliceWidth*w,_sliceHeight*h,_sliceWidth,_sliceHeight));
			slice->setAnchorPoint(Vec2(0,0));
			slice->setPosition(w*_sliceWidth,size.height-(h+1)*_sliceHeight);
			this->addChild(slice);
			_slices[w][h] = slice;
		}
	}
	return true;
}

void cuttingPhotos::initTouchEventlistener()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch *touch,Event *event){
		Point locationInNode = this->convertToNodeSpace(touch->getLocation());
		Size s = this->getContentSize();
		Rect rect = Rect(0,0,s.width,s.height);
		if(rect.containsPoint(locationInNode)){
			return true;
		}else{	return false;}
	};

	listener->onTouchEnded = [&](Touch *touch,Event *event){
		Point locationInNode = this->convertToNodeSpace(touch->getLocation());
		//������ĸ�ͼ��
		int x = static_cast<int>(floor(locationInNode.x/this->_sliceWidth));
		int y = static_cast<int>(floor(locationInNode.y/this->_sliceHeight));

		this->move(x,y);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
}

void cuttingPhotos::move(int x , int y)
{
	y = _columns - y - 1;
	auto slice = _slices[x][y];
	if(slice == nullptr){
		return;
	}

	//������Χ�Ƿ��пհ׵�����
	bool isFind = false;
	Point targetPos;
	if(y < _columns - 1 && _slices[x][y+1] == nullptr){
		targetPos.x = x;
		targetPos.y = y+1;
		isFind = true;
	}else if (y>0 && _slices[x][y-1]==nullptr) {
        targetPos.x=x;
        targetPos.y=y-1;
        isFind=true;
    }else if (x<_rows-1 && _slices[x+1][y]==nullptr) {
        targetPos.x=x+1;
        targetPos.y=y;
        isFind=true;
    }else if (x>0 && _slices[x-1][y]==nullptr) {
        targetPos.x=x-1;
        targetPos.y=y;
        isFind=true;
    }
	if (isFind==false) {
        return;
    }

	slice->runAction(MoveTo::create(0.25, Vec2(targetPos.x*_sliceWidth,_sliceHeight*_columns-(targetPos.y+1)*_sliceHeight)));
    _slices[targetPos.x][targetPos.y]=slice;
    _slices[x][y]=nullptr;
}
void cuttingPhotos::restart()
{
	Vector<Sprite*> list;
	//ȡ��������Ƭ
	for(int w = 0; w < _rows; w++){
		for (int h=0; h<_columns; ++h) {
			auto slice = _slices[w][h];
			if(slice!=nullptr){
				list.pushBack(slice);
			}
		}
	}

	 //������õ�ÿһ��λ��
    for (int w=0; w<_rows; ++w) {
        for (int h=0; h<_columns; ++h) {
            
            if (w==_rows-1 && h==_columns-1) {
                //���һ�鲻��Ҫ
                _slices[w][h]=nullptr;
                break;
            }
            
            auto length= list.size();
            
            auto value= static_cast<int>(CCRANDOM_0_1()*length);
            
            auto slice=list.at(value);
            list.eraseObject(slice);
            
            _slices[w][h]=slice;
            
            slice->setPosition(w*_sliceWidth,_sliceHeight*_columns-(h+1)*_sliceHeight);
        }
    }
}

