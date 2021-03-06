#pragma once
#include "Components.h"
#include "Scenes.h"
#include "PresentFarmScene.h"
#include "Game.h"

class CharacterCreateScene : public Scene {
public:
	SDL_Color WHITE = { 255,255,255,255 };
	SDL_Color RED = { 255,0,0,255 };

	CharacterCreateScene() = default;
	~CharacterCreateScene() {}

	void init() override {
		initialized = true;

		Entity * back = &manager->createEntity<CharacterCreateScene>();
		back->addComponent<UILabel>(40, 40, "<- Back", "vt323", WHITE);
		back->addComponent<UIButton>([&]() {
			printf("Go to main menu\n");
			//manager->changeScene<MainMenuScene>();
			manager->prevScene();
		});

		Entity * hsv = &manager->createEntity<CharacterCreateScene>();
		hsv->addComponent<UILabel>(600, 200, "Hair Color", "vt323", WHITE);
		hsv->addComponent<UIHueSaturationValue>(720, 203, 18, 175);

		name = &manager->createEntity<CharacterCreateScene>();
		name->addComponent<UILabel>(100, 100, "Name", "vt323", WHITE);
		name->addComponent<TextBoxComponent>(150, 100, 16, "vt323", WHITE, RED);

		// TODO: 10 is a magic number add it to properties
		playerSkin = &manager->createEntity<CharacterCreateScene>();
		playerSkin->addComponent<TransformComponent>(-55.0f, -25.0f, 32, 16, 10);
		playerSkin->addComponent<SpriteComponent>("skin1");
		playerSkin->addComponent<UILabel>(200, 230, "Skin Color", "vt323", WHITE);
		playerSkin->addComponent<UISelector>("skin", "vt323", 10, 510, 230, 325, 230);

		playerEyes = &manager->createEntity<CharacterCreateScene>();
		playerEyes->addComponent<TransformComponent>(-55.0f, -25.0f, 32, 16, 10);
		playerEyes->addComponent<SpriteComponent>("eyes1");
		playerEyes->addComponent<UILabel>(200, 260, "Eyes", "vt323", WHITE);
		playerEyes->addComponent<UISelector>("eyes", "vt323", 5, 510, 260, 325, 260);

		playerHair = &manager->createEntity<CharacterCreateScene>();
		playerHair->addComponent<TransformComponent>(-55.0f, -25.0f, 32, 16, 10);
		playerHair->addComponent<SpriteComponent>("hair1");
		playerHair->addComponent<UILabel>(200, 200, "Hair", "vt323", WHITE);
		playerHair->addComponent<UISelector>("hair", "vt323", 10, 510, 200, 325, 200);

		create = &manager->createEntity<CharacterCreateScene>();
		create->addComponent<UILabel>(600, 550, "CREATE", "vt323", WHITE);
		create->addComponent<UIButton>([&]() {
			if (name->getComponent<TextBoxComponent>().getText().length() < 3) {
				name->getComponent<TextBoxComponent>().displayError("Name must be 3 characters");
			} else {
				// get all the currently selected stuff and go!
				std::string playerName = createPlayerTexture();
				int eyeIndex = playerEyes->getComponent<UISelector>().getSelectedIndex();

				manager->addScene<PresentFarmScene>(playerName, "eyes" + std::to_string(eyeIndex));
				manager->changeScene<PresentFarmScene>();
			}
		});

		back->addGroup(Game::groupUI);
		hsv->addGroup(Game::groupUI);
		name->addGroup(Game::groupUI);
		playerSkin->addGroup(Game::groupUI);
		playerEyes->addGroup(Game::groupUI);
		playerHair->addGroup(Game::groupUI);
		create->addGroup(Game::groupUI);
	}

	void update() override {
		manager->updateEntities<CharacterCreateScene>();
	}

	void render() override {
		manager->renderEntities<CharacterCreateScene>();
	}

	void unload() override {
		manager->destroyEntities<CharacterCreateScene>();
	}

private:
	Entity* create;
	Entity* name;

	Entity* playerSkin;
	Entity* playerEyes;
	Entity* playerHair;

	Entity* accessories;
	Entity* top;
	Entity* bottom;
	Entity* shoes;

	Entity* accessorySelector;
	Entity* topSelector;
	Entity* bottomSelector;
	Entity* shoeSelector;

	std::string createPlayerTexture() {
		SDL_Rect dest;
		int skinIndex = playerSkin->getComponent<UISelector>().getSelectedIndex();
		SDL_QueryTexture(Game::assets->GetTexture("skin" + std::to_string(skinIndex)), NULL, NULL, &dest.w, &dest.h);

		SDL_Texture*  target_texture = SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dest.w, dest.h);

		dest.x = 0;
		dest.y = 0;

		SDL_SetRenderTarget(Game::renderer, target_texture);
		SDL_RenderCopy(Game::renderer, Game::assets->GetTexture("skin" + std::to_string(skinIndex)), NULL, NULL);

		int hairIndex = playerHair->getComponent<UISelector>().getSelectedIndex();
		SDL_RenderCopy(Game::renderer, Game::assets->GetTexture("hair" + std::to_string(hairIndex)), NULL, NULL);

		SDL_SetRenderTarget(Game::renderer, NULL);

		// might not do this
		std::string playerName = name->getComponent<TextBoxComponent>().getText();
		std::string fileName("Assets\\SavedAssets\\" + playerName + ".png");
		Utils::saveTexturePng(Game::renderer, target_texture, fileName.c_str());
		Game::assets->AddTexture(playerName, fileName.c_str());
		return playerName;
	}
};
