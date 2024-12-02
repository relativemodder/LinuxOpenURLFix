#include <Geode/Geode.hpp>
#include <Geode/modify/CCApplication.hpp>

#include <relative.linuxapi/include/linuxapi.hpp>

using namespace geode::prelude;



class $modify(FixedCCApplication, CCApplication) {
	void openURL(const char* url) {
		LinuxAPI::getInstance()->open(url)
		.listen([] (geode::Result<bool>* resultPtr) {
			if (!resultPtr->isOk()) {
				FLAlertLayer::create(
					"Error opening URL",
					resultPtr->unwrapErr(),
					"OK"
				)->show();
				return;
			}
		});
	}
};


void detourOpenLinkInBrowser(const std::string& url) {
	CCApplication::get()->openURL(url.c_str());
}


$execute {
	Mod::get()->hook(
		reinterpret_cast<void*>(
			geode::addresser::getNonVirtual(
				geode::modifier::Resolve<const std::string&>::func(&geode::utils::web::openLinkInBrowser)
			)
		),
		&detourOpenLinkInBrowser,
		"geode::utils::web::openLinkInBrowser",
		tulip::hook::TulipConvention::Stdcall		
	).unwrap();
}