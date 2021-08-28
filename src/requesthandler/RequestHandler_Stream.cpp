#include "RequestHandler.h"
#include "../plugin-macros.generated.h"

RequestResult RequestHandler::GetStreamStatus(const Request& request)
{
	OBSOutputAutoRelease streamOutput = obs_frontend_get_streaming_output();

	json responseData;
	responseData["outputActive"] = obs_output_active(streamOutput);
	responseData["outputReconnecting"] = obs_output_reconnecting(streamOutput);
	responseData["outputTimecode"] = Utils::Obs::StringHelper::GetOutputTimecodeString(streamOutput);
	responseData["outputDuration"] = Utils::Obs::NumberHelper::GetOutputDuration(streamOutput);
	responseData["outputBytes"] = (uint64_t)obs_output_get_total_bytes(streamOutput);
	responseData["outputSkippedFrames"] = obs_output_get_frames_dropped(streamOutput);
	responseData["outputTotalFrames"] = obs_output_get_total_frames(streamOutput);

	return RequestResult::Success(responseData);
}

RequestResult RequestHandler::StartStream(const Request& request)
{
	if (obs_frontend_streaming_active())
		return RequestResult::Error(RequestStatus::StreamRunning);

	// TODO: Call signal directly to perform blocking wait
	obs_frontend_streaming_start();

	return RequestResult::Success();
}

RequestResult RequestHandler::StopStream(const Request& request)
{
	if (!obs_frontend_streaming_active())
		return RequestResult::Error(RequestStatus::StreamNotRunning);

	// TODO: Call signal directly to perform blocking wait
	obs_frontend_streaming_stop();

	return RequestResult::Success();
}