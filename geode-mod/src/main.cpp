#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class $modify(MyAIEditorUI, EditorUI) {
    EventListener<web::WebTask> m_webListener;

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        auto sprite = ButtonSprite::create("AI Gen", "goldFont.fnt", "GJ_button_01.png", 0.8f);
        auto btn = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(MyAIEditorUI::onAIGenerateClicked)
        );
        btn->setID("ai-generator-button"_spr);

        auto menu = this->getChildByID("bottom-menu");
        if (menu) {
            menu->addChild(btn);
            menu->updateLayout();
        } else {
            this->addChild(btn);
            btn->setPosition({ 100, 100 });
        }

        return true;
    }

    void onAIGenerateClicked(CCObject* sender) {
        auto editor = m_editorLayer;
        if (!editor || !editor->m_level) return;

        std::string levelName = editor->m_level->m_levelName;
        std::string songPath = editor->m_level->getAudioFileName();

        FLAlertLayer::create("AI Generator", "Contacting Python AI Backend...", "OK")->show();

        matjson::Value body;
        body["level_name"] = levelName;
        body["audio_path"] = songPath;
        body["speed_mode"] = 1;

        m_webListener.bind([this, editor](web::WebTask::Event* e) {
            if (web::WebResponse* res = e->getValue()) {
                if (!res->ok()) {
                    FLAlertLayer::create("Error", "Could not connect to Python AI Server! Check server.py.", "OK")->show();
                    return;
                }

                auto jsonRes = res->json().unwrapOrDefault();
                if (!jsonRes.contains("objects")) {
                    FLAlertLayer::create("Error", "Invalid JSON format received from backend.", "OK")->show();
                    return;
                }

                int spawnedCount = 0;
                auto objects = jsonRes["objects"];

                if (objects.isArray()) {
                    for (auto& item : objects.asArray().unwrapOrDefault()) {
                        int id = item["id"].asInt().unwrapOrDefault();
                        float x = static_cast<float>(item["x"].asDouble().unwrapOrDefault());
                        float y = static_cast<float>(item["y"].asDouble().unwrapOrDefault());

                        auto obj = editor->createObject(id, ccp(x, y), false);
                        if (obj) spawnedCount++;
                    }
                }

                std::string msg = "Successfully generated " + std::to_string(spawnedCount) + " layout objects!";
                FLAlertLayer::create("AI Success", msg.c_str(), "OK")->show();
            }
        });

        web::WebRequest req;
        req.bodyJSON(body);
        m_webListener.setFilter(req.post("http://127.0.0.1:8000/generate"));
    }
};
