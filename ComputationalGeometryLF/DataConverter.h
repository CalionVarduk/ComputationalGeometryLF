#pragma once

#include "CartesianPlane.h"
#include "CV++ Utilities\CVFileIO.h"

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace System::Collections::Generic;
	using namespace CVFileIO;
	
	public ref class DataConverter : public DataConverterBase
	{
		public:
			DataConverter() : DataConverterBase() {}

		protected:
			virtual bool writeDataToBytes() override
			{
				List<GeometricObject^>^ objects = interpretDataAs<List<GeometricObject^>^>();
				writeInt(objects->Count);
				_writeObjectTypes(objects);
				_writeObjects(objects);
				return true;
			}

			virtual bool readDataFromBytes() override
			{
				Int32 count;
				if(!_readObjectCount(count)) return false;

				List<GeometricObject^>^ objects = _readNewObjectList(count);
				if(!_readObjectTypes(objects, count)) return false;

				_readObjects(objects);
				return true;
			}

			virtual Int32 dataBinaryLength() override
			{
				List<GeometricObject^>^ objects = interpretDataAs<List<GeometricObject^>^>();

				Int32 fileSize = 4 +							// object count
								 (objects->Count * 4) +			// type flags
								 (objects->Count * 16);			// origin points

				for(Int32 i = 0; i < objects->Count; ++i) {
					if(!objects[i]->isPoint()) {
						if(objects[i]->isPolygon())
							fileSize += 8 + objects[i]->interpretAs<GeometricPolygon>()->PointCount * 16;
						else fileSize += 16;					// need two more doubles for each object that is not a point or polygon
					}
				}

				return fileSize;
			}

		private:
			void _writeObjectTypes(List<GeometricObject^>^ objects)
			{
				for(Int32 i = 0; i < objects->Count; ++i) {
					GeometricObject^ obj = objects[i];
					if(obj->isPoint()) writeInt(1);
					else if(obj->isSegment()) writeInt(2);
					else if(obj->isRectangle()) writeInt(4);
					else if(obj->isPolygon()) {
						writeInt(8);
						writeInt(obj->interpretAs<GeometricPolygon>()->PointCount);
					}
				}
			}

			void _writeObjects(List<GeometricObject^>^ objects)
			{
				for(Int32 i = 0; i < objects->Count; ++i) {
					GeometricObject^ obj = objects[i];
					if(obj->isPoint()) _writePoint(obj);
					else if(obj->isSegment()) _writeSegment(obj);
					else if(obj->isRectangle()) _writeRectangle(obj);
					else if(obj->isPolygon()) _writePolygon(obj);
				}
			}

			void _writePoint(GeometricObject^ obj)
			{
				GeometricPoint^ p = obj->interpretAs<GeometricPoint>();
				writeDouble(p->X);
				writeDouble(p->Y);
			}

			void _writeSegment(GeometricObject^ obj)
			{
				GeometricSegment^ s = obj->interpretAs<GeometricSegment>();
				writeDouble(s->StartX);
				writeDouble(s->StartY);
				writeDouble(s->EndX);
				writeDouble(s->EndY);
			}

			void _writeRectangle(GeometricObject^ obj)
			{
				GeometricRectangle^ r = obj->interpretAs<GeometricRectangle>();
				writeDouble(r->Left);
				writeDouble(r->Bottom);
				writeDouble(r->Width);
				writeDouble(r->Height);
			}

			void _writePolygon(GeometricObject^ obj)
			{
				GeometricPolygon^ p = obj->interpretAs<GeometricPolygon>();
				writeInt(p->PointCount);
				for(Int32 i = 0; i < p->PointCount; ++i) {
					writeDouble(p->getPointAt(i).X);
					writeDouble(p->getPointAt(i).Y);
				}
			}

			bool _readObjectCount(Int32% count)
			{
				if(BytesLength < 4) return false;
				count = readInt();
				return true;
			}

			List<GeometricObject^>^ _readNewObjectList(Int32 count)
			{
				List<GeometricObject^>^ objects = gcnew List<GeometricObject^>(count);
				DataObject = objects;
				return objects;
			}

			bool _readObjectTypes(List<GeometricObject^>^ objects, Int32 count)
			{
				Int32 sizeCheck = 4 + (count * 4) + (count * 16);
				if(BytesLength < sizeCheck) return false;

				for(Int32 i = 0; i < count; ++i) {
					Int32 type = readInt();
					if(type == 1) _addPoint(objects);
					else if(type == 2) {
						sizeCheck += 16;
						_addSegment(objects);
					}
					else if(type == 4) {
						sizeCheck += 16;
						_addRectangle(objects);
					}
					else if(type == 8) {
						sizeCheck += 8 + readInt() * 16;
						_addPolygon(objects);
					}
				}

				if(BytesLength != sizeCheck) return false;
				return true;
			}

			void _addPoint(List<GeometricObject^>^ objects)
			{
				GeometricPoint^ p = gcnew GeometricPoint();
				p->MainColor = Color::Red;
				objects->Add(p);
			}

			void _addSegment(List<GeometricObject^>^ objects)
			{
				GeometricSegment^ s = gcnew GeometricSegment();
				s->MainColor = Color::Blue;
				//s->DrawStyleHint = GeometricObject::DrawStyle::DrawAndFill;
				objects->Add(s);
			}

			void _addRectangle(List<GeometricObject^>^ objects)
			{
				GeometricRectangle^ r = gcnew GeometricRectangle();
				r->MainColor = Color::ForestGreen;
				//r->DrawStyleHint = GeometricObject::DrawStyle::DrawAndFill;
				objects->Add(r);
			}

			void _addPolygon(List<GeometricObject^>^ objects)
			{
				GeometricPolygon^ p = gcnew GeometricPolygon();
				p->MainColor = Color::LightCoral;
				//p->DrawStyleHint = GeometricObject::DrawStyle::DrawAndFill;
				objects->Add(p);
			}

			void _readObjects(List<GeometricObject^>^ objects)
			{
				for(Int32 i = 0; i < objects->Count; ++i) {
					GeometricObject^ obj = objects[i];
					if(obj->isPoint()) _readPoint(obj);
					else if(obj->isSegment()) _readSegment(obj);
					else if(obj->isRectangle()) _readRectangle(obj);
					else if(obj->isPolygon()) _readPolygon(obj);
				}
			}

			void _readPoint(GeometricObject^ obj)
			{
				GeometricPoint^ p = obj->interpretAs<GeometricPoint>();
				p->X = readDouble();
				p->Y = readDouble();
			}

			void _readSegment(GeometricObject^ obj)
			{
				GeometricSegment^ s = obj->interpretAs<GeometricSegment>();
				s->StartX = readDouble();
				s->StartY = readDouble();
				s->EndX = readDouble();
				s->EndY = readDouble();
			}

			void _readRectangle(GeometricObject^ obj)
			{
				GeometricRectangle^ r = obj->interpretAs<GeometricRectangle>();
				r->Left = readDouble();
				r->Bottom = readDouble();
				r->Width = readDouble();
				r->Height = readDouble();
			}

			void _readPolygon(GeometricObject^ obj)
			{
				GeometricPolygon^ p = obj->interpretAs<GeometricPolygon>();
				Int32 pCount = readInt();
				for(Int32 i = 0; i < pCount; ++i) {
					double x = readDouble();
					double y = readDouble();
					p->addPoint(x, y);
				}
			}
	};
}