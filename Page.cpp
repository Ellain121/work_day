#include "Page.hpp"

#include <cassert>

Page::Context::Context(MainWindow& mWindow, ProgramData& progData)
	: mainWindow{&mWindow}
	, progData{&progData}
{
}

Page::Page(Page::Context context, QWidget* parent)
	: QWidget{parent}
	, mContext{context}
{
}

Page::Context Page::getContext() const
{
	return mContext;
}

void Page::addProgDataEntry(ProgDataType type, const QVariant& var)
{
	assert(!mContext.progData->contains(type) &&
		   "ProgDataEntry cannot be added! (Already exist)");

	(*mContext.progData)[type] = var;
}

void Page::updateProgDataEntry(ProgDataType type, const QVariant& var)
{
	assert(mContext.progData->contains(type) && "ProgDataEntry not found! (Not exist)");

	(*mContext.progData)[type] = var;
}

QVariant Page::getProgDataEntry(ProgDataType type) const
{
	assert((mContext.progData != nullptr) && "progDate = nullptr!");

	auto& map = *mContext.progData;
	assert((map.find(type) != map.end()) && "ProgData: Cannot find such type!");

	return map[type];
}
