-- cmd�����¼�
CMD_TALK = 1         --����������ָ
CMD_SPECIAL = 2      --�������ϵ�ָ��
CMD_CLICK = 3        --����ҳ���ϵ�ָ��

-- guideType����������
GUIDE_TOCLICKTASK = 1    --ָ������ť��ָ
GUIDE_TECH_SURE = 2      --�о��Ƽ�ȷ�ϰ�ť
GUIDE_GOTO_SOCIETY = 3   --���簴ť��ָ
GUIDE_CLCLK_HEAD = 4     --������ͷ��
GUIDE_CLCLK_DRESS = 5    --����·�����
GUIDE_DRESS = 6          --װ������
GUIDE_GET = 7            --����
GUIDE_RECRUIT = 8        --��ļʿ��
GUIDE_CHOOSE = 9         --ѡ��ŵ�
GUIDE_MAKE = 10          --����
GUIDE_CHOOSE_WEAPON = 11 --�����������
GUIDE_CPOY = 12          --������ť��ָ
GUIDE_BACK = 13          --�سǰ�ť��ָ
GUIDE_UPGRADE = 14       --��������
GUIDE_CHOOSE_MA = 15     --ѡ����
GUIDE_DRESS_MA = 16      --�������
GUIDE_GET_HERO = 17      --��ļ�佫��ť
GUIDE_CLCLK_ZL = 18      --�������ͷ��
GUIDE_CHOOSE_CLOUTH = 19 --ѡ�����Ӽ�
GUIDE_CHOOSE_HEAD = 20   --ѡ������
GUIDE_MAKE_CHOOSE = 21   --ѡ������
GUIDE_MAKE_UP = 22       --��������
GUIDE_DRESS_HEAD = 23    --���ͷ������
GUIDE_AUTOBUILDING = 24  --�Զ�����
GUIDE_HERODLG = 25       --�佫��ť
GUIDE_TECH = 26          --�Ƽ���Ч
GUIDE_WORK = 27          --����ť
GUIDE_COPY_HERO = 28     --�����佫��ļ��ָ
GUIDE_DRESS_YIN = 29     --���ӡ�´���
GUIDE_DRESS_FU = 30      --���������
GUIDE_GUOQI = 31         --�������1
GUIDE_DAZAO = 32         --�����ʼ�������
GUIDE_MAKE_POS = 33      --ָ��������
GUIDE_CHOOSE_FU = 34     --ѡ�������
GUIDE_CHOOSE_YIN = 35    --ѡ��ӡ����
GUIDE_ZJ = 36            --���͹�ָ���Ž�
GUIDE_SZ = 37 			 --����ť

GUIDE_TASK_FINISH = 111  --������ɣ���Ϊһ�������ı�ʶ
GUIDE_END = 999          --ָ��������ʶ

local m_uiFinger = {nil,nil,nil,nil,nil,nil}; --UnityEngine.GameObject

-- �򿪽���
function GuideDlgOpen()
	m_Dlg = eye.uiManager:Open( "GuideDlg" );
end

-- ���ؽ���
function GuideDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "GuideDlg" );
end

-- ɾ������
function GuideDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- �¼�
----------------------------------------

-- ������ť���ʱ����
function GuideDlgOnEvent( nType, nControlID, value, gameObject )
end

-- ����ʱ����
function GuideDlgOnAwake( gameObject )
	-- �ؼ���ֵ	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiFinger[1] = objs[0];
	m_uiFinger[2] = objs[1];
	m_uiFinger[3] = objs[2];
	m_uiFinger[4] = objs[3];	
	m_uiFinger[5] = objs[4];
	m_uiFinger[6] = objs[5];
end

-- �����ʼ��ʱ����
function GuideDlgOnStart( gameObject )
	
end

-- ������ʾʱ����
function GuideDlgOnEnable( gameObject )
	
end

-- ��������ʱ����
function GuideDlgOnDisable( gameObject )
	
end

-- ����ɾ��ʱ����
function GuideDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- ÿ֡����
function GuideDlgOnLogic( gameObject )
	
end

local mId = 0
local mStep = 0
local mIsGuiding = false


-- �򿪽���
function Guide(id, step, force)
	m_Dlg = eye.uiManager:Open( "GuideDlg" );	
	if force ~= true then
        if id == 0 then                   -- ��������״̬
            if step ~= 1 then
                return;
            end
        else
            if id ~= id then              -- �ڽ�����������
                return;
            elseif step ~= mStep + 1 then  -- ���費��
                return;
            end
        end

        -- ��������Ƿ����
        if GuideCheck( id ) and mStep == 999 then
			mIsGuiding = false;
            return;
        end   
    end
	
	mIsGuiding = true;
	
	mId = id;
	mStep = step;
	warn(mId);
	warn(step);
	
	GuideClear();
	currentFinger = nil;
	currentMainFinger = nil;
	
	if g_guide[mId][mStep].cmd == CMD_SPECIAL or g_guide[mId][mStep].cmd == CMD_TALK then FindCmdTpye(nil) end
end

function GuideNext()
	if table.getn(g_guide[mId]) == mStep then
		GuideClear();
		system_askinfo( ASKINFO_GUAID, "", mId + 1 );
	else
		Guide( mId, mStep + 1 );
	end
end

function GuideEnd()
	GuideClear();
	mStep = 0;
	system_askinfo( ASKINFO_GUAID, "", mId + 1 );
end

function GuideCheck(id)
	if id == table.getn(g_guide) then 
		GuideClear();
		return true
	else 
		return false
	end
end

function IsGuiding()
	return mIsGuiding
end

function GuideClear()
	HideGuideFinger();
	HideCurrentFinger();
end

function GetCurrentGuideType()
	if mStep == 0 then
		return 0;
	end
	return g_guide[mId][mStep].guideType
end

function GetGuideSpecialEvent()
	return g_guide[mId][mStep].isSpecial
end
function HideGuideFinger()
	if m_Dlg ~= nil then
		for i = 1, table.getn(m_uiFinger), 1 do 
			if m_uiFinger[i] then
				SetFalse(m_uiFinger[i].transform);
			end
		end
	end
end

function ShowGuideFinger()
	HideGuideFinger();
	if currentMainFinger ~= nil then
		SetTrue(currentMainFinger.transform);
	end
end

function HideCurrentFinger()
	if currentFinger ~= nil and currentFinger.transform ~= nil then
		SetFalse(currentFinger.transform);
		currentFinger = nil;
	end
end

-- ���õ�ǰָ��λ��
function FindCmdTpye(tran)
	if g_guide[mId] == nil or g_guide[mId][mStep] == nil then
		return
	end
	cmd = g_guide[mId][mStep].cmd;
	point = g_guide[mId][mStep].point;
	deviation = Vector3.New(g_guide[mId][mStep].x,g_guide[mId][mStep].y,0);
	if cmd == 2 then 
		currentMainFinger = m_uiFinger[point];
		ShowGuideFinger();
		if g_guide[mId][mStep].guideType == GUIDE_TOCLICKTASK then 
			m_uiFinger[point].transform.position = GetQuestDlgPos() + deviation;
		elseif g_guide[mId][mStep].guideType == GUIDE_CPOY then 
			m_uiFinger[point].transform.position = GetCopyPos() + deviation;
		elseif g_guide[mId][mStep].guideType == GUIDE_GOTO_SOCIETY then 
			m_uiFinger[point].transform.position = GetWorldPos() + deviation;
			
		elseif g_guide[mId][mStep].guideType == GUIDE_BACK then
			m_uiFinger[point].transform.position = GetBackPos() + deviation;
		elseif g_guide[mId][mStep].guideType == GUIDE_AUTOBUILDING then
			m_uiFinger[point].transform.position = GetAutoBuildPos() + deviation;
		elseif g_guide[mId][mStep].guideType == GUIDE_HERODLG then 
			m_uiFinger[point].transform.position = GetHeroDlgPos() + deviation;
		elseif g_guide[mId][mStep].guideType == GUIDE_WORK then 
			m_uiFinger[point].transform.position = GetWorkPos() + deviation;
		end
	elseif cmd == 3 then
		if currentFinger == nil and m_uiFinger[point] then
			currentFinger = addChild(tran,m_uiFinger[point]);
			currentFinger.transform.position = currentFinger.transform.position + deviation;
		end
	elseif cmd == 1 then
		
	end
end

