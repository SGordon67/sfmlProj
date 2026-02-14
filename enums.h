#ifndef ENUMS_H
#define ENUMS_H

enum class GameState
{
    MainMenu,
    Playing,
    Paused,
    GameOver
};

enum class RenderLayer
{
	FarBackground,
	CloseBackground,
	Main,
	Foreground
};

enum class ShapeType
{
	Circle,
	Rectangle
};

enum class Button
{
    Escape,
    Up,
    Down,
    Left,
    Right,
    Interact,
    Tab,
    Count
};

#endif
