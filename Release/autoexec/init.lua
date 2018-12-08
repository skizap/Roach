-- Farewell Infortality.
-- Version: 2.82
-- Instances:
local Injected = Instance.new("ScreenGui")
local Frame = Instance.new("Frame")
local Header = Instance.new("TextLabel")
local Body = Instance.new("TextLabel")
local Close = Instance.new("TextButton")
--Properties:
Injected.Name = "Injected"
Injected.Parent = game.Players.LocalPlayer:WaitForChild("PlayerGui")

Frame.Parent = Injected
Frame.BackgroundColor3 = Color3.new(0.301961, 0.301961, 0.301961)
Frame.Position = UDim2.new(0.5, -200, 0.5, -75)
Frame.Size = UDim2.new(0, 400, 0, 150)
Frame.Style = Enum.FrameStyle.RobloxRound

Header.Name = "Header"
Header.Parent = Frame
Header.BackgroundTransparency = 1
Header.Size = UDim2.new(1, 0, 0, 30)
Header.Font = Enum.Font.ArialBold
Header.Text = "Welcome!"
Header.TextColor3 = Color3.new(1, 1, 1)
Header.TextSize = 24
Header.TextStrokeColor3 = Color3.new(0, 0.501961, 0)
Header.TextStrokeTransparency = 0.80000001192093

Body.Name = "Body"
Body.Parent = Frame
Body.BackgroundTransparency = 1
Body.Position = UDim2.new(0, 44, 0, 50)
Body.Size = UDim2.new(0.767499983, 0, 0.866666675, -70)
Body.Font = Enum.Font.Arial
Body.Text = "Welcome to roach. Created By X-Tools"
Body.TextColor3 = Color3.new(1, 1, 1)
Body.TextSize = 18
Body.TextWrapped = true
Body.TextXAlignment = Enum.TextXAlignment.Left
Body.TextYAlignment = Enum.TextYAlignment.Top

Close.Name = "Close"
Close.Parent = Frame
Close.Position = UDim2.new(0.25, 0, 1, -40)
Close.Size = UDim2.new(0.5, 0, 0, 40)
Close.Modal = true
Close.Style = Enum.ButtonStyle.RobloxButtonDefault
Close.Font = Enum.Font.Arial
Close.Text = "Close"
Close.TextColor3 = Color3.new(1, 1, 1)
Close.TextSize = 18
-- Scripts:
function SCRIPT_UOSF81_FAKESCRIPT() -- Injected.WelcomeGUI 
	getfenv().script = Instance.new('LocalScript', Injected)

	--[[
		Welcome GUI
		version: 2
	
		By: Legend26
	
		The GUI is given to a player once when they join the game.
	
		Note: I had noticed people had used this. I first created this when GUIs had
			just been released. As such, it was really old, but due to how popular 
			the model is, I decided to update it.
	
		Usage: Modify the GUI's text as you please and keep the entire model in workspace.
	]]
	
	-------------
	--Variables--
	-------------
	
	local Gui = script.Parent
	
	---------
	--Setup--
	---------
	while not Gui or not Gui:findFirstChild"Frame" do
		wait()
		Gui = script.Parent
	end
	
	--------
	--Main--
	--------
	
	for i,v in next, Gui.Frame:GetChildren() do
		v.Visible = false
	end
	
	local OSize = Gui.Frame.Size
	local OPos = Gui.Frame.Position
	Gui.Frame.Position = Gui.Frame.Position + UDim2.new(0, Gui.Frame.AbsoluteSize.X/2, 0, Gui.Frame.AbsoluteSize.Y/2)
	Gui.Frame.Size = UDim2.new(0,0,0,0)
	
	Gui.Frame.Visible = true
	Gui.Frame:TweenSizeAndPosition(OSize,OPos)
	wait(1)
	
	for i,v in next, Gui.Frame:GetChildren() do
		v.Visible = true
	end
	
	Gui.Frame.Close.MouseButton1Click:connect(function ()
		for i,v in next, Gui.Frame:GetChildren() do
			v.Visible = false
		end
		Gui.Frame:TweenSizeAndPosition(UDim2.new(0,0,0,0),Gui.Frame.Position + UDim2.new(0, Gui.Frame.AbsoluteSize.X/2, 0, Gui.Frame.AbsoluteSize.Y/2))
		wait(1)
		Gui:Destroy()
	end)
	
	--Legend26

end




coroutine.resume(coroutine.create(SCRIPT_UOSF81_FAKESCRIPT))
