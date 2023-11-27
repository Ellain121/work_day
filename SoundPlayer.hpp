#pragma once

#include <QMediaPlayer>
#include <QSoundEffect>
#include <memory>

class SoundPlayer
{
public:
	enum Type
	{
		TimeIsUp
	};

public:
	static void initSound();
	static void playSound(SoundPlayer::Type soundType);

private:
	static std::unique_ptr<QSoundEffect> mSoundPlayer;
	static bool							 mPlayerActive;
};
