#include <Geode/Geode.hpp>

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


#include <Geode/modify/CCMenuItemSpriteExtra.hpp>
class $modify(CCEryItem, CCMenuItemSpriteExtra) {
	bool init(CCNode* a, CCNode* b, CCObject* rec, SEL_MenuHandler selector) {
		if (!CCMenuItemSpriteExtra::init(a, b, rec, selector)) {
			return false;
		}

        queueInMainThread([item = Ref(this)](){
            auto spr = CCSprite::create("eri.png"_spr);
            spr->setScaleX(item->getContentWidth() / spr->getContentWidth());
            spr->setScaleY(item->getContentHeight() / spr->getContentHeight());
            item->addChildAtPosition(spr, Anchor::Center, {}, false);
        });

		return true;
	}

    void activate() {
        if (random::generate<int>(0,2)) {
            CCMenuItemSpriteExtra::activate();
        } else {
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
        }
    }
};
