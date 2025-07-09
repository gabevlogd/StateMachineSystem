// Copyright Epic Games, Inc. All Rights Reserved.

#include "StateMachineSystem.h"

#include "GameplayTagsManager.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FStateMachineSystemModule"

void FStateMachineSystemModule::StartupModule()
{
	// Get the plugin object
	TSharedPtr<IPlugin> ThisPlugin = IPluginManager::Get().FindPlugin(TEXT("StateMachineSystem"));
	check(ThisPlugin.IsValid());

	// Add the plugin's config path for tag loading
	UGameplayTagsManager::Get().AddTagIniSearchPath(ThisPlugin->GetBaseDir() / TEXT("Config") / TEXT("Tags"));
}

void FStateMachineSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FStateMachineSystemModule, StateMachineSystem)