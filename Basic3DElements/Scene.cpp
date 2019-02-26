#include "sceneElements.h"



Scene::Scene()
{
}


Scene::~Scene()
{
}

Camera* Scene::getActiveCam()
{
	return getCam(actualCam);
}

string Scene::getActiveCamName()
{
	return actualCam;
}

void Scene::registerCam(Camera *cam, string name)
{
	camTable[name] = cam;
	actualCam = name;
	changeCam(name);

}

void Scene::changeCam(string name)
{
	Camera *cam = getCam(name);
	actualCam = name;

	map<string, SceneObjectBase*>::iterator i = objectTable.begin();

	while (i != objectTable.end())
	{
		cam->registerSceneObject(i->second);
		i++;
	}
}

void Scene::deleteCam(string name)
{
	map<string, Camera*>::iterator i = camTable.begin();
	int deleted = 0;
	while (i != camTable.end())
	{
		
		if (i->first == name)
		{
			camTable.erase(i);
			changeCam(camTable.begin()->first);
			return;
		}
			
		i++;
	}
}

Camera* Scene::getCam(string name)
{
	map<string, Camera*>::iterator i = camTable.begin();

	while (i != camTable.end())
	{
		if (i->first == name)
			return i->second;
		i++;
	}
	return 0;
}

void Scene::addSceneObject(SceneObjectBase *object, string name)
{
	Camera *cam = getCam(actualCam);
	objectTable[name] = object;

	cam->registerSceneObject(object);
}

void Scene::deleteSceneObject(string name)
{
	map<string, SceneObjectBase*>::iterator i = objectTable.begin();

	while (i != objectTable.end())
	{
		if (i->first == name)
		{
			objectTable.erase(i);
			return;
		}
		i++;
	}
}

SceneObjectBase* Scene::getSceneObject(string name)
{
	map<string, SceneObjectBase*>::iterator i = objectTable.begin();

	while (i != objectTable.end())
	{
		if (i->first == name)
			return i->second;
		i++;
	}
	return 0;
}

void Scene::draw()
{
	map<string, SceneObjectBase*>::iterator i = objectTable.begin();

	while (i != objectTable.end())
	{
		i->second->draw();
		i++;
	}
}