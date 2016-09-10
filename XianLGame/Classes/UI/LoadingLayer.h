#ifndef __libCommon_LoadingLayer_h__
#define __libCommon_LoadingLayer_h__

#include "Base/YZLayer.h"
#include <string>

namespace YZ
{

	class LoadingLayer : public YZLayer
	{
	private:
		int		_opacity;
		Label*	_label;

	public:
		static LoadingLayer* createLoading(Node* parent, const TTFConfig& ttfConfig, const std::string& text, const std::string& image);
	
		static LoadingLayer* createLoading(const TTFConfig& ttfConfig, const std::string& text, const std::string& image, const std::string& imageBG);

		static void removeLoading(Node* parent);

	private:
		LoadingLayer(); 
		virtual ~LoadingLayer();

	public:
		virtual bool init() override;

		// 功能接口
	public:
		void setText(const std::string& text);

	private:
		void createDialog(const TTFConfig& ttfConfig, const std::string& text, const std::string& image);

		void createDialog(const TTFConfig& ttfConfig, const std::string& text, const std::string& image, const std::string& imageBG);
	
		void autoSetOpacity(float dt);

	public:
		CREATE_FUNC(LoadingLayer);
	};

}
#endif // __libCommon_LoadingLayer_h__
