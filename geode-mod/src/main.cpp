#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class $modify(MyAIEditorUI, EditorUI) {
    // Event listener required for modern Geode async web requests in 2.208
    EventListener<web::WebTask> m_webListener;

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        // 1. Create custom AI Generator Button UI
        auto sprite = ButtonSprite::create("AI Gen", "goldFont.fnt", "GJ_button_01.png", 0.8f);
        auto btn = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(MyAIEditorUI::onAIGenerateClicked)
        );
        btn->setID("ai-generator-button"_spr);

        // 2. Attach button to editor bottom bar
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

        FLAlertLayer::create("AI Generator", "Analyzing audio & generating layout...", "OK")->show();

        // 3. Construct JSON Payload
        matjson::Value body = matjson::Object {
            {"level_name", levelName},
            {"audio_path", songPath},
            {"speed_mode", 1}
        };

        // 4. Register Modern Geode Web Event Listener (GD 2.208)
        m_webListener.bind([this, editor](web::WebTask::Event* e) {
            if (web::WebResponse* res = e->getValue()) {
                if (!res->ok()) {
                    FLAlertLayer::create("Error", "Could not connect to Python AI Server! (Is server.py running?)", "OK")->show();
                    return;
                }

                auto jsonRes = res->json().unwrapOrDefault();
                if (!jsonRes.contains("objects")) {
                    FLAlertLayer::create("Error", "Invalid layout format received from server.", "OK")->show();
                    return;
                }

                auto objectsArray = jsonRes["objects"].asArray().unwrapOrDefault();
                int spawnedCount = 0;

                // 5. Instantiates objects onto the editor grid
                for (auto& item : objectsArray) {
                    int id = item["id"].asInt().unwrapOrDefault();
                    float x = static_cast<float>(item["x"].asDouble().unwrapOrDefault());
                    float y = static_cast<float>(item["y"].asDouble().unwrapOrDefault());

                    auto obj = editor->createObject(id, ccp(x, y), false);
                    if (obj) spawnedCount++;
                }

                std::string msg = "Successfully generated " + std::to_string(spawnedCount) + " objects!";
                FLAlertLayer::create("AI Success", msg.c_str(), "OK")->show();
            } 
            else if (e->isCancelled()) {
                FLAlertLayer::create("Cancelled", "AI Generation task was cancelled.", "OK")->show();
            }
        });

        // Send POST request
        web::WebRequest req;
        req.bodyJSON(body);
        m_webListener.setFilter(req.post("http://127.0.0.1:8000/generate"));
    }
};