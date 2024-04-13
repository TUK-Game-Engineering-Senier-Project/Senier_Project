#pragma once

const ULONG MAX_SAMPLE_COUNT = 50; // 50ȸ�� ������ ó���ð��� �����Ͽ� ����Ѵ�.

class CGameTimer
{
public:
	CGameTimer();
	virtual ~CGameTimer();

	// Ÿ�̸��� �ð��� �����Ѵ�.
	void Tick(float fLockFPS = 0.0f);

	void Start();
	void Stop();
	void Reset();

	// ������ ����Ʈ�� ��ȯ�Ѵ�.
	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);
	// �������� ��սð��� ��ȯ�Ѵ�.
	float GetTimeElapsed();
	float GetTotalTime();

private:
	// Scale Counter�� ��
	double							m_fTimeScale;
	// ������ ������ ���� ������ �ð�
	float							m_fTimeElapsed;

	__int64							m_nBasePerformanceCounter;
	__int64							m_nPausedPerformanceCounter;
	__int64							m_nStopPerformanceCounter;
	__int64							m_nCurrentPerformanceCounter;
	__int64							m_nLastPerformanceCounter;

	// ��ǻ���� Performance Frequency
	__int64							m_nPerformanceFrequencyPerSec;

	// ������ �ð�
	__int64 m_nCurrentTime;
	// ������ �������� �ð�
	__int64 m_nLastTime;


	// ������ �ð��� �����ϱ� ���� �迭
	float							m_fFrameTime[MAX_SAMPLE_COUNT];
	// ������ ������ Ƚ��
	ULONG							m_nSampleCount;

	// ������ ������ ����Ʈ
	unsigned long					m_nCurrentFrameRate;
	// �ʴ� ������ ��
	unsigned long					m_nFramesPerSecond;
	// ������ ����Ʈ ��� �ҿ� �ð�
	float							m_fFPSTimeElapsed;

	bool							m_bStopped;

};

