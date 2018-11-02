// Copyright 2001-2018 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "MainThread.h"
#include "Request.h"
#include <CryAudio/IAudioSystem.h>
#include <CrySystem/ISystem.h>
#include <CrySystem/TimeValue.h>
#include <CryInput/IInput.h>
#include <concqueue/concqueue.hpp>

namespace CryAudio
{
class CSystem final : public IAudioSystem, public ::ISystemEventListener, public IInputEventListener
{
public:

	CSystem() = default;
	virtual ~CSystem() override;

	CSystem(CSystem const&) = delete;
	CSystem(CSystem&&) = delete;
	CSystem& operator=(CSystem const&) = delete;
	CSystem& operator=(CSystem&&) = delete;

	// CryAudio::IAudioSystem
	virtual void        Release() override;
	virtual void        SetImpl(Impl::IImpl* const pIImpl, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        LoadTrigger(ControlId const triggerId, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        UnloadTrigger(ControlId const triggerId, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        ExecuteTriggerEx(SExecuteTriggerData const& triggerData, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        ExecuteTrigger(ControlId const triggerId, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        StopTrigger(ControlId const triggerId = InvalidControlId, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        SetParameter(ControlId const parameterId, float const value, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        SetSwitchState(ControlId const switchId, SwitchStateId const switchStateId, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        PlayFile(SPlayFileInfo const& playFileInfo, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        StopFile(char const* const szName, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        ReportStartedFile(CStandaloneFile& standaloneFile, bool const bSuccessfullyStarted, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        ReportStoppedFile(CStandaloneFile& standaloneFile, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        ReportFinishedEvent(CEvent& event, bool const bSuccess, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        ReportVirtualizedEvent(CEvent& event, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        ReportPhysicalizedEvent(CEvent& event, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        StopAllSounds(SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        Refresh(char const* const szLevelName, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        PreloadSingleRequest(PreloadRequestId const id, bool const bAutoLoadOnly, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        UnloadSingleRequest(PreloadRequestId const id, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        LoadSetting(ControlId const id, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        UnloadSetting(ControlId const id, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        ReloadControlsData(char const* const szFolderPath, char const* const szLevelName, SRequestUserData const& userData = SRequestUserData::GetEmptyObject()) override;
	virtual void        AddRequestListener(void (*func)(SRequestInfo const* const), void* const pObjectToListenTo, ESystemEvents const eventMask) override;
	virtual void        RemoveRequestListener(void (*func)(SRequestInfo const* const), void* const pObjectToListenTo) override;
	virtual void        ExternalUpdate() override;
	virtual char const* GetConfigPath() const override;
	virtual IListener*  CreateListener(CTransformation const& transformation, char const* const szName = nullptr) override;
	virtual void        ReleaseListener(IListener* const pIListener) override;
	virtual IObject*    CreateObject(SCreateObjectData const& objectData = SCreateObjectData::GetEmptyObject()) override;
	virtual void        ReleaseObject(IObject* const pIObject) override;
	virtual void        GetFileData(char const* const szName, SFileData& fileData) override;
	virtual void        GetTriggerData(ControlId const triggerId, STriggerData& triggerData) override;
	virtual void        GetImplInfo(SImplInfo& implInfo) override;
	virtual void        Log(ELogType const type, char const* const szFormat, ...) override;

	// Below data is only used when INCLUDE_AUDIO_PRODUCTION_CODE is defined!
	virtual void ExecutePreviewTrigger(ControlId const triggerId) override;
	virtual void ExecutePreviewTriggerEx(Impl::ITriggerInfo const& triggerInfo) override;
	virtual void StopPreviewTrigger() override;
	// ~CryAudio::IAudioSystem

	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener

	// IInputEventListener
	virtual bool OnInputEvent(SInputEvent const& event) override;
	// ~IInputEventListener

	bool Initialize();
	void PushRequest(CRequest const& request);
	void ParseControlsData(char const* const szFolderPath, EDataScope const dataScope, SRequestUserData const& userData = SRequestUserData::GetEmptyObject());
	void ParsePreloadsData(char const* const szFolderPath, EDataScope const dataScope, SRequestUserData const& userData = SRequestUserData::GetEmptyObject());
	void AutoLoadSetting(EDataScope const dataScope, SRequestUserData const& userData = SRequestUserData::GetEmptyObject());
	void RetriggerControls(SRequestUserData const& userData = SRequestUserData::GetEmptyObject());
	void InternalUpdate();

private:

	using Requests = ConcQueue<UnboundMPSC, CRequest>;
	using RequestsSyncCallbacks = ConcQueue<UnboundSPSC, CRequest>;

	void           ReleaseImpl();
	void           OnLanguageChanged();
	void           ProcessRequests(Requests& requestQueue);
	void           ProcessRequest(CRequest& request);
	ERequestStatus ProcessManagerRequest(CRequest const& request);
	ERequestStatus ProcessCallbackManagerRequest(CRequest& request);
	ERequestStatus ProcessObjectRequest(CRequest const& request);
	ERequestStatus ProcessListenerRequest(SRequestData const* const pPassedRequestData);
	void           NotifyListener(CRequest const& request);
	ERequestStatus HandleSetImpl(Impl::IImpl* const pIImpl);
	ERequestStatus HandleRefresh(char const* const szLevelName);
	void           SetImplLanguage();
	void           SetCurrentEnvironmentsOnObject(CObject* const pObject, EntityId const entityToIgnore);
	void           SetOcclusionType(CObject& object, EOcclusionType const occlusionType) const;
	void           ExecuteDefaultTrigger(EDefaultTriggerType const type, SRequestUserData const& userData = SRequestUserData::GetEmptyObject());

	static void    OnCallback(SRequestInfo const* const pRequestInfo);

	bool                  m_isInitialized = false;
	bool                  m_didThreadWait = false;
	volatile float        m_accumulatedFrameTime = 0.0f;
	std::atomic<uint32>   m_externalThreadFrameId{ 0 };
	uint32                m_lastExternalThreadFrameId = 0;
	uint32                m_objectPoolSize = 0;
	uint32                m_eventPoolSize = 0;
	SImplInfo             m_implInfo;
	CMainThread           m_mainThread;

	Requests              m_requestQueue;
	RequestsSyncCallbacks m_syncCallbacks;
	CRequest              m_syncRequest;
	CRequest              m_request;
	CryEvent              m_mainEvent;
	CryEvent              m_audioThreadWakeupEvent;

#if defined(ENABLE_AUDIO_LOGGING)
	int m_loggingOptions;
#endif // ENABLE_AUDIO_LOGGING

#if defined(INCLUDE_AUDIO_PRODUCTION_CODE)
public:
	void ScheduleIRenderAuxGeomForRendering(IRenderAuxGeom* pRenderAuxGeom);

private:
	void SubmitLastIRenderAuxGeomForRendering();
	void DrawDebug();
	void HandleDrawDebug();
	void HandleRetriggerControls();

	std::atomic<IRenderAuxGeom*> m_currentRenderAuxGeom{ nullptr };
	std::atomic<IRenderAuxGeom*> m_lastRenderAuxGeom{ nullptr };
#endif // INCLUDE_AUDIO_PRODUCTION_CODE
};
} // namespace CryAudio