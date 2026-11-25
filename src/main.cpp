#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <alphalaneous.editortab_api/API.hpp>

using namespace geode::prelude;

namespace {
    constexpr int PHOTO_NODE_TAG = 7001;

    // Fit the photo into the tab's available area and center it.
    void layoutPhoto(alpha::editor_tabs::EditorTab* tabNode) {
        if (!tabNode) return;
        auto photo = tabNode->getChildByTag(PHOTO_NODE_TAG);
        if (!photo) return;

        CCSize area = tabNode->getContentSize();
        if (area.width <= 0.f || area.height <= 0.f) {
            area = alpha::editor_tabs::EditorTab::getAvailableSize();
        }

        auto size = photo->getContentSize();
        if (size.width <= 0.f || size.height <= 0.f) return;

        float scale = std::min(area.width / size.width, area.height / size.height);
        photo->setScale(scale);
        photo->setAnchorPoint({0.5f, 0.5f});
        photo->setPosition({area.width / 2.f, area.height / 2.f});
    }
}

class $modify(PhotoTabEditorUI, EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        auto buildMode = alpha::editor_tabs::getMode(alpha::editor_tabs::Build);
        if (!buildMode) return true;

        // Tab content: an empty (item-less) tab with our picture as a child
        auto tabNode = alpha::editor_tabs::EditorTab::create();

        auto photo = CCSprite::create("tab-photo.png"_spr);
        if (photo) {
            photo->setTag(PHOTO_NODE_TAG);
            tabNode->addChild(photo);
        }

        // Tab icon (the button you click to open the tab)
        auto icon = CCSprite::create("tab-icon.png"_spr);
        if (!icon) icon = CCSprite::createWithSpriteFrameName("GJ_button_01.png");
        else icon->setScale(0.3f);

        auto tab = buildMode->createTab("photo-tab"_spr, tabNode, icon);

        layoutPhoto(tabNode);

        tabNode->addEventListener(
            alpha::editor_tabs::Tab::SwitchTabEvent(tab),
            [tabNode](bool show) {
                if (show) layoutPhoto(tabNode);
            }
        );

        return true;
    }
};
