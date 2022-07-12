set renderer=src\engine\renderer\renderer.c
set events=src\engine\input\input.c src\engine\window_events\window_events.c
set io=src\engine\io\io.c
set scene_manager=src\engine\scene_manager\scene_manager.c
set core=src\engine\core\core.c
set ecs=src\engine\ecs\ecs.c
set audio=src\engine\audio\audio.c
set logger=src\engine\logger\logger.c

set files=src\main.c src\engine\global.c %renderer% %events% %io% %core% %ecs% %scene_manager% %audio% %logger%

set libs=D:\NyxEngine\lib\SDL2main.lib D:\NyxEngine\lib\SDL2.lib D:\NyxEngine\lib\SDL2_image.lib D:\NyxEngine\lib\SDL2_mixer.lib

CL /Zi /I D:\NyxEngine\include %files% /link %libs% /OUT:nyx_game.exe
