//------------------------------------------------------------------------
// Game.cpp
//------------------------------------------------------------------------

#include "AudioEngine.h"

float gameTime;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------

void Init()
{
	gameTime = 0;

	AudioEngine& audioEngine = AudioEngine::Instance();

	// Init FMOD
	audioEngine.Init();

	// Load a sound bank
	audioEngine.LoadBank("Master");

	// Create a music event instance									 // Right-click your event in fmod Studio -> Copy GUID
	AudioEvent& music = AudioEngine::Instance().CreateEvent("UniqueName", "{b56cb9d2-1d47-4099-b80e-7d257b99a823}");

	// Play the music event
	music.Play();
}

//------------------------------------------------------------------------
// Update your game here. 
//------------------------------------------------------------------------
void Update(float deltaTime)
{

	// Increment game time
	gameTime += deltaTime;

	// Get a ref to the engine
	AudioEngine& audioEngine = AudioEngine::Instance();

	// Get a ref to the music event
	AudioEvent& music = audioEngine.GetEvent("UniqueName");

	// Get listener
	AudioListener& listener = audioEngine.GetListener();
	
	
	// After 5 seconds go underwater
	if (gameTime > 4.0f)
	{
		music.SetPosition(glm::vec3(10, 0, 0));
	}

	// After 10 second slow down
	if (gameTime > 10.0f)
	{
		audioEngine.SetGlobalParameter("Timewarp", 0);
		music.SetParameter("Underwater", 1);
	}


	// Update fmod
	audioEngine.Update();

}


//------------------------------------------------------------------------
// Add your display calls here
//------------------------------------------------------------------------
void Render()
{
	// Who needs graphics when you have audio?
}


//------------------------------------------------------------------------
// Add your shutdown code here.
//------------------------------------------------------------------------
void Shutdown()
{
	AudioEngine::Instance().Shutdown();

}