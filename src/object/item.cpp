#include "object/item.h"
#include "object/object_attack.h"
#include "util/tokenreader.h"
#include "util/token.h"
#include "util/bitmap.h"
#include "util/ebox.h"
#include <iostream>
#include <string>

using namespace std;

Item::Item( const string & filename ) throw( LoadException ):
collide( 0 ){
	TokenReader tr( filename );

	setHealth( 1 );

	Token * head;
	try{
		head = tr.readToken();
	} catch( const TokenException & ex ){
		cerr<< "Could not read "<<filename<<" : "<<ex.getReason()<<endl;
		// delete head;
		throw LoadException("Could not open character file");
	}

	if ( *head != "item" ){
		throw new LoadException( "Item does not begin with 'item'" );
	}

	while ( head->hasTokens() ){
		Token * next = head->readToken();
		if ( *next == "frame" ){
			string file;
			*next >> file;
			picture.load( file );
		}
	}

	collide = new ECollide( picture );
}

Item::Item( const Item & item ):
collide( 0 ){
	this->picture = item.picture;
	collide = new ECollide( this->picture );
	setHealth( item.getHealth() );
	setX( item.getX() );
	setY( item.getY() );
	setZ( item.getZ() );
}
	
Object * Item::copy(){
	return new Item( *this );
}
	
bool Item::isGettable(){
	return true;
}

ECollide * Item::getCollide() const {
	return collide;
}
	
bool Item::collision( ObjectAttack * obj ){
	if ( getAlliance() == obj->getAlliance() ){
		return false;
	}

	if ( this->getCollide() != 0 && obj->getCollide() != 0 ){
		bool my_xflip = false;
		bool his_xflip = false;
		if ( getFacing() == FACING_LEFT )
			my_xflip = true;
		if ( obj->getFacing() == FACING_LEFT )
			his_xflip = true;

		int mx, my;
		int ax, ay;

		ECollide * me = getCollide();
		ECollide * him = obj->getCollide();

		mx = this->getRX() - getWidth() / 2;
		my = this->getRY() - getHeight();
		ax = obj->getRX() - obj->getWidth() / 2;
		ay = obj->getRY() - obj->getHeight();

		return ( me->Collision( him, mx, my, ax, ay, my_xflip, false, his_xflip, false ) );

}
return false;


}

void Item::act( vector< Object * > * others, World * world ){
	// cout << "tough actin tinactin: " << this << endl;
}

void Item::draw( Bitmap * work, int rel_x ){
	// cout << "draw item at " << getRX() - rel_x << " " << getRY() << endl;
	picture.draw( getRX() - rel_x, getRY(), *work );
}

bool Item::isCollidable( Object * obj ){
	return false;
}

const int Item::getWidth() const {
	return picture.getWidth();
}

const int Item::getHeight() const {
	return picture.getHeight();
}
	
Item::~Item(){
	delete collide;
}
