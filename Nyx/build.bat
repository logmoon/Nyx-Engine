set renderer=src\engine\renderer\renderer.c src\engine\renderer\animator.c
set events=src\engine\input\input.c src\engine\window_events\window_events.c
set io=src\engine\io\io.c
set scene_manager=src\engine\scene_manager\scene_manager.c
set core=src\engine\core\core.c
set physics=src\engine\physics\physics.c
set ecs=src\engine\ecs\ecs.c
set audio=src\engine\audio\audio.c
set logger=src\engine\logger\logger.c
set menu=src\engine\menu\menu.c

set game=src\game\sandbox\scene.c src\game\sandbox\main_scene.c

set files=src\game\main.c src\engine\global.c %renderer% %events% %io% %core% %physics% %ecs% %scene_manager% %audio% %logger% %menu% %game%

set libs=D:\NyxEngine\lib\SDL2main.lib D:\NyxEngine\lib\SDL2.lib D:\NyxEngine\lib\SDL2_image.lib D:\NyxEngine\lib\SDL2_mixer.lib D:\NyxEngine\lib\SDL2_ttf.lib

CL /Zi /I D:\NyxEngine\include %files% /link %libs% /OUT:nyx_game.exe
