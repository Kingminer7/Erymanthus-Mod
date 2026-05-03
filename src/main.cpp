#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/utils/string.hpp"
#include <Geode/Geode.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/utils/VMTHookManager.hpp>
#include <alphalaneous.alphas_geode_utils/include/ObjectModify.hpp>

using namespace geode::prelude;

    std::vector<std::string> reasons = {
        "Consists primarily AI generated presses",
        "Doesn't use the geode utils that you would know about if you spent 5 seconds reading the docs",
        "Uses scary methods to activate the buttons",
        "Low effort",
        "Button does not work on macOS",
        "Button has a 0.1% chance to break"
    };

    std::vector<std::function<std::string ()>> special = {
        []{
            auto mods = Loader::get()->getAllMods();
            return fmt::format("Contains idea stolen from {}", mods.at(random::generate<int>(0, mods.size()))->getName());  
        }
    };

template <string::ConstexprString S, typename T>
T const& getSettingFast() {
    static T value = (
        listenForSettingChanges<T>(S.data(), [](T val) {
            value = std::move(val);
        }),
        getMod()->getSettingValue<T>(S.data())
    );
    return value;
}

bool rejectPress() {
    if (random::generate<int>(0, 100) < getSettingFast<"chance", int>()) return false;
    FMODAudioEngine::get()->playEffect("s14061.ogg"_spr);
    std::string str = "";
    for (int i = 0; i < random::generate(2, 4); i++) {
        if (i > 0) str += "\n\n";
        auto r = random::generate(0, reasons.size() + special.size() * 2);
        if (r >= reasons.size()) {
            r -= reasons.size();
            str += special[r / 2]();
        } else {
            str += reasons[r];
        }
    }
    MDPopup::create("Your press was rejected by Ery.", str, "Ok")->show();
    return true;
}

void addErySprite(CCNode* node) {
    queueInMainThread([item = Ref(node)](){
        auto spr = CCSprite::create("eri.png"_spr);
        spr->setScaleX(item->getContentWidth() / spr->getContentWidth());
        spr->setScaleY(item->getContentHeight() / spr->getContentHeight());
        item->addChildAtPosition(spr, Anchor::Center, {}, false);
    });
}

#include <Geode/modify/CCMenuItem.hpp>
class $modify(CCEryItem, CCMenuItem) {
	bool initWithTarget(CCObject* rec, SEL_MenuHandler sel) {
		if (!CCMenuItem::initWithTarget(rec, sel)) {
			return false;
		}

        addErySprite(this);
		return true;
	}

    void activate() {
        // ccmenuitemspriteextra breaks kinda when its not the orig method hooked
        if (getUserFlag("mise")) return CCMenuItem::activate();
        if (!rejectPress()) CCMenuItem::activate();
    }
};

#include <Geode/modify/CCMenuItemSpriteExtra.hpp>
class $modify(CCEryItemSpriteExtra, CCMenuItemSpriteExtra) {
    bool init(CCNode* a, CCNode* b, CCObject* rec, SEL_MenuHandler sel) {
        if (!CCMenuItemSpriteExtra::init(a,b,rec,sel)) return false;
        setUserFlag("mise"_spr);
        return true;
    }

    void activate() {
        if (!rejectPress()) CCMenuItemSpriteExtra::activate();
    }
};

void eryGeodeButton(Button* btn) {
    if (!rejectPress()) btn->activate();
}

class $classModify(EryButton, geode::Button) {
    void modify() {
        // if (Loader::get()->isPatchless()) return log::warn("No geode::Button support, jitless detected.");
        addErySprite(this);

        auto hook = VMTHookManager::get().addHook<ResolveC<EryButton>::func(&EryButton::activate)>(this, "geode::Button::activate");
    }

    void activate() override {
        if (!rejectPress()) Button::activate();
    }
};
