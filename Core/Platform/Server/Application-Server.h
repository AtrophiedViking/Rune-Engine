#include <string>

namespace Rune {

	struct ApplicationConfig {
		std::string Name;
	};
	
	class Application {
	public:
		
		Application(const ApplicationConfig& config);
		
		~Application();
		
		virtual void PushLayer(class Layer layer);
		
		virtual void Run();



		static Application& Get();
	private:
		ApplicationConfig m_Config;
	};
}