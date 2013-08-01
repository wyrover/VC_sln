#ifndef _ROVERLIB_FACTORYMANAGER_H_
#define _ROVERLIB_FACTORYMANAGER_H_

#include <string>
#include <map>

namespace roverlib {

	/************************************************************************/
	/* 产品接口                                                                     */
	/************************************************************************/
	class IFactoryItem
	{
	public:
		IFactoryItem() { }
		virtual ~IFactoryItem() { }
	};


	/************************************************************************/
	/* 工厂接口                                                                     */
	/************************************************************************/
	class IFactory
	{
	public:
		IFactory() { }
		virtual ~IFactory() { }

		virtual IFactoryItem* CreateItem() = 0;
	};

	/************************************************************************/
	/* 模板工厂                                                                     */
	/************************************************************************/
	template <typename Type>
	class FactoryTemplate :	public IFactory
	{
	public:
		FactoryTemplate() { }
		virtual ~FactoryTemplate() { }

		virtual IFactoryItem* CreateItem()
		{
			IFactoryItem* result = new Type();
			return result;
		}
	};


	/************************************************************************/
	/* 工厂管理器                                                                     */
	/************************************************************************/
	class FactoryManager
	{
	public:
		FactoryManager()
		{
		}
		
		~FactoryManager()
		{
		}

		static FactoryManager* GetInstancePtr()
		{
			static FactoryManager instance;
			return &instance;
		}

		static FactoryManager& GetInstance()
		{
			return *GetInstancePtr();
		}

		bool ExistFactory(const std::string& _factoryName)
		{
			return mFactories.find(_factoryName) != mFactories.end();
		}

		void RegisterFactory(IFactory* _factory, const std::string& _factoryName)
		{
			mFactories[_factoryName] = _factory;
		}

		void UnregisterAllFactories()
		{
			for (MapFactory::iterator factory = mFactories.begin(); factory != mFactories.end(); factory ++)
				delete (*factory).second;
			mFactories.clear();
		}

		IFactoryItem* CreateItem(const std::string& _factoryName)
		{
			MapFactory::iterator item = mFactories.find(_factoryName);
			if (item != mFactories.end())
				return (*item).second->CreateItem();
			return 0;
		}

		template <typename Type>
		Type* CreateItem(const std::string& _factoryName)
		{
			IFactoryItem* item = CreateItem(_factoryName);
			if (item != nullptr)
			{
				Type* result = dynamic_cast<Type*>(item);
				if (result != nullptr)
					return result;
				delete item;
			}
			return nullptr;
		}

	private:
		typedef std::map<std::string, IFactory*> MapFactory;
		MapFactory mFactories;
	};


	bool IsExistFactoryName(const std::string& _factoryName)
	{
		return FactoryManager::GetInstancePtr()->ExistFactory(_factoryName);
	}

	void RegisterFactory(IFactory* _factory, const std::string& _factoryName)
	{
		FactoryManager::GetInstancePtr()->RegisterFactory(_factory, _factoryName);
	}

	/************************************************************************/
	/* 工厂注册器                                                                     */
	/************************************************************************/
	template <typename Type>
	class FactoryItemRegistrator
	{
	public:
		FactoryItemRegistrator(const std::string& _factoryName) :
		  mFactoryName(_factoryName)
		  {
			  if (!IsExistFactoryName(mFactoryName))
			  {
				  IFactory* factory = new FactoryTemplate<Type>();
				  RegisterFactory(factory, mFactoryName);
			  }
		  }

	private:
		FactoryItemRegistrator() { }

	private:
		std::string mFactoryName;
	};

/************************************************************************/
/*  两个工厂注册宏                                                                    */
/************************************************************************/
#define FACTORY_ITEM_ATTRIBUTE(type) namespace { static FactoryItemRegistrator<type> instance(#type); }

#define FACTORY_ITEM(type) RegisterFactory(new FactoryTemplate<type>(), #type);
	

}

#endif