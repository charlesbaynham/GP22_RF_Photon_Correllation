///*
//||
//|| @file HashMap.h
//|| @version 1.0 Beta
//|| @author Alexander Brevig
//|| @contact alexanderbrevig@gmail.com
//||
//|| @description
//|| | This library provides a simple interface for storing data with an associate key
//|| #
//||
//|| @license
//|| | This library is free software; you can redistribute it and/or
//|| | modify it under the terms of the GNU Lesser General Public
//|| | License as published by the Free Software Foundation; version
//|| | 2.1 of the License.
//|| |
//|| | This library is distributed in the hope that it will be useful,
//|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
//|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//|| | Lesser General Public License for more details.
//|| |
//|| | You should have received a copy of the GNU Lesser General Public
//|| | License along with this library; if not, write to the Free Software
//|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//|| #
//||
//*/
//
//#ifndef HASHMAP_H
//#define HASHMAP_H
//
//#include <Arduino.h>
//
///* Handle association */
//// template<typename hash,typename map>
//template<typename map>
//
//class HashType {
//public:
//	HashType(){ reset(); }
//	
//	HashType(const char* code,map value):hashCode(code),mappedValue(value){}
//	
//	// void reset(){ hashCode = 0; mappedValue = 0; }
//	void reset(){ hashCode = 0; }
//	const char* getHash(){ return hashCode; }
//	void setHash(const char* code){ hashCode = code; }
//	map getValue(){ return mappedValue; }
//	void setValue(map value){ mappedValue = value; }
//	
//	HashType& operator()(const char* code, map value){
//		setHash( code );
//		setValue( value );
//	}
//private:
//	const char* hashCode;
//	map mappedValue;
//};
//
///*
//Handle indexing and searches
//TODO - extend API
//*/
//template<typename map>
//class HashMap {
//public:
//	HashMap(HashType<map>* newMap,byte newSize){
//		hashMap = newMap;
//		size = newSize;
//		for (byte i=0; i<size; i++){
//			hashMap[i].reset();
//		}
//	}
//	
//	HashType<map>& operator[](int x){
//		//TODO - bounds
//		return hashMap[x];
//	}
//	
//	byte getIndexOf( const char* key ){
//		for (byte i=0; i<size; i++){
//			if (hashCompare(hashMap[i].getHash(), key)){
//				return i;
//			}
//		}
//	}
//	map getValueOf( const char* key ){
//		for (byte i=0; i<size; i++){
//			if (hashCompare(hashMap[i].getHash(), key)){
//				return hashMap[i].getValue();
//			}
//		}
//	}
//	bool exists( const char* key ){
//
//		for (byte i=0; i<size; i++){
//			if (hashCompare(hashMap[i].getHash(), key)){
//				return true;
//			}
//		}
//		return false;
//	}
//	
//	void debug(){
//		for (byte i=0; i<size; i++){
//			Serial.print(hashMap[i].getHash());
//			Serial.print(" - ");
//			Serial.println(hashMap[i].getValue());
//		}
//	}
//
//private:
//	HashType<map>* hashMap;
//	byte size;
//
//	bool hashCompare(const char* a, const char* b) {
//		return strcicmp(a, b) == 0;
//	}
//
//	// Case insensitive strcmp
//	int strcicmp(char const *a, char const *b)
//	{
//	    for (;; a++, b++) {
//	        int d = tolower(*a) - tolower(*b);
//	        if (d != 0 || !*a)
//	            return d;
//	    }
//	}
//};
//
//#endif
//
///*
//|| @changelog
//|| | 1.0 2009-07-13 - Alexander Brevig : Initial Release
//|| #
//*/
