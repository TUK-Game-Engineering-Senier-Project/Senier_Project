#pragma once

const ULONG MAX_SAMPLE_COUNT = 50; // 50ȸ�� ������ ó���ð��� �����Ͽ� ����Ѵ�.

class CGameTimer
{
public:
	CGameTimer();
	virtual ~CGameTimer();

	void Start() { }
	void Stop() { }
	void Reset();
	// Ÿ�̸��� �ð��� �����Ѵ�.
	void Tick(float fLockFPS = 0.0f);
	// ������ ����Ʈ�� ��ȯ�Ѵ�.
	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);
	// �������� ��սð��� ��ȯ�Ѵ�.
	float GetTimeElapsed();

private:
	// ��ǻ�Ͱ� Performance Counter�� ������ �ִ� ��
	bool m_bHardwareHasPerformanceCounter;
	// Scale Counter�� ��
	float m_fTimeScale;
	// ������ ������ ���� ������ �ð�
	float m_fTimeElapsed;
	// ������ �ð�
	__int64 m_nCurrentTime;
	// ������ �������� �ð�
	__int64 m_nLastTime;
	// ��ǻ���� Performance Frequency
	__int64 m_nPerformanceFrequency;

	// ������ �ð��� �����ϱ� ���� �迭
	float m_fFrameTime[MAX_SAMPLE_COUNT];
	// ������ ������ Ƚ��
	ULONG m_nSampleCount;

	// ������ ������ ����Ʈ
	unsigned long m_nCurrentFrameRate;
	// �ʴ� ������ ��
	unsigned long m_nFramesPerSecond;
	// ������ ����Ʈ ��� �ҿ� �ð�
	float m_fFPSTimeElapsed;

	bool m_bStopped;

};

