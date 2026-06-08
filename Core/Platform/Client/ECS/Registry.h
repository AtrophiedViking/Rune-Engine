#include "Entity.h"

namespace VECS
{
	class Registry 
	{
	public:
		EntityId Create();
        void Destroy(EntityId id);

        bool Alive(EntityId id) const;

    public:
        template<typename T, typename... Args>
        T& Emplace(EntityId id, Args&&... args);

        template<typename T>
        T* TryGet(EntityId id);

        template<typename T>
        void Remove(EntityId id);

        template<typename... Ts>
        void Each(std::invocable<EntityId, Ts&...> auto fn);  // iterate entities that have ALL of Ts

    private:
        std::vector<uint32_t>          m_Generations;
        std::vector<EntityId>          m_FreeList;
    };
}