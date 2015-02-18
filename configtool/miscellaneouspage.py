
import wx
from configtool.page import Page
from configtool.data import reFloat


class MiscellaneousPage(wx.Panel, Page):
  def __init__(self, parent, nb, idPg):
    wx.Panel.__init__(self, nb, wx.ID_ANY)
    Page.__init__(self)
    self.parent = parent
    self.id = idPg

    self.labels = {'USE_INTERNAL_PULLUPS': "Use Internal Pullups",
                   'EECONFIG': "Enable EEPROM Storage",
                   'DEBUG': "Turn on Debugging",
                   'BANG_BANG': "Enable",
                   'BANG_BANG_ON': "On PWM Level:",
                   'BANG_BANG_OFF': "Off PWM Level:",
                   'MOVEBUFFER_SIZE': "Movebuffer Size:",
                   'DC_EXTRUDER': "Heater:", 'DC_EXTRUDER_PWM': "PWM:",
                   'USE_WATCHDOG': "Use the Watchdog Timer",
                   'REFERENCE': "Analog Reference:",
                   'STEP_INTERRUPT_INTERRUPTIBLE': "STEP Interrupt",
                   'TH_COUNT': "Temperature History Size:",
                   'FAST_PWM': "Fast PWM",
                   'ENDSTOP_STEPS': "Endstop Steps:",
                   'PID_SCALE': "PID Scaling Factor:",
                   'TEMP_HYSTERESIS': "Temperature Hysteresis:",
                   'TEMP_RESIDENCY_TIME': "Temperature Residency Time:",
                   'TEMP_EWMA': "Temperature EWMA:",
                   'HEATER_SANITY_CHECK': "Heater Sanity Check"}

    self.heaterNameNone = "<none>"
    self.heaterNames = [self.heaterNameNone]
    self.boardHeaters = []
    self.processors = []

    self.defaultRef = 'REFERENCE_AVCC'
    self.references = [self.defaultRef, 'REFERENCE_AREF',
                       'REFERENCE_1V1', 'REFERENCE_2V56']

    sz = wx.GridBagSizer()
    sz.AddSpacer((20, 40), pos = (0, 0))
    sz.AddSpacer((40, 40), pos = (0, 2))
    sz.AddSpacer((40, 40), pos = (0, 4))
    sz.AddSpacer((20, 30), pos = (1, 0))
    sz.AddSpacer((20, 30), pos = (2, 0))
    sz.AddSpacer((20, 30), pos = (3, 0))
    sz.AddSpacer((20, 30), pos = (4, 0))
    sz.AddSpacer((20, 30), pos = (5, 0))
    sz.AddSpacer((20, 30), pos = (6, 0))
    sz.AddSpacer((20, 30), pos = (7, 0))
    sz.AddSpacer((20, 30), pos = (8, 0))

    labelWidth = 140

    k = 'EECONFIG'
    cb = self.addCheckBox(k, self.onCheckBox)
    sz.Add(cb, pos = (1, 1))

    k = 'USE_INTERNAL_PULLUPS'
    cb = self.addCheckBox(k, self.onCheckBox)
    sz.Add(cb, pos = (2, 1))

    k = 'DEBUG'
    cb = self.addCheckBox(k, self.onCheckBox)
    sz.Add(cb, pos = (3, 1))

    k = 'USE_WATCHDOG'
    cb = self.addCheckBox(k, self.onCheckBox)
    sz.Add(cb, pos = (4, 1))

    k = 'STEP_INTERRUPT_INTERRUPTIBLE'
    cb = self.addCheckBox(k, self.onCheckBox)
    sz.Add(cb, pos = (5, 1))

    k = 'FAST_PWM'
    cb = self.addCheckBox(k, self.onCheckBox)
    sz.Add(cb, pos = (6, 1))

    k = 'XONXOFF'
    cb = self.addCheckBox(k, self.onCheckBox)
    sz.Add(cb, pos = (7, 1))

    k = 'HEATER_SANITY_CHECK'
    cb = self.addCheckBox(k, self.onCheckBox)
    sz.Add(cb, pos = (8, 1))

    k = 'REFERENCE'
    ch = self.addChoice(k, self.references,
                        self.references.index(self.defaultRef),
                        labelWidth, self.onChoice)
    sz.Add(ch, pos = (1, 3))

    b = wx.StaticBox(self, wx.ID_ANY, "BANG BANG Bed Control")
    sbox = wx.StaticBoxSizer(b, wx.VERTICAL)
    sbox.AddSpacer((5, 5))

    k = 'BANG_BANG'
    cb = self.addCheckBox(k, self.onCheckBox)
    sbox.Add(cb, 1, wx.LEFT, 60)
    sbox.AddSpacer((5, 20))

    k = 'BANG_BANG_ON'
    tc = self.addTextCtrl(k, 80, self.onTextCtrlInteger)
    sbox.Add(tc)
    sbox.AddSpacer((5, 5))

    k = 'BANG_BANG_OFF'
    tc = self.addTextCtrl(k, 80, self.onTextCtrlInteger)
    sbox.Add(tc)
    sbox.AddSpacer((5, 5))

    sz.Add(sbox, pos = (3, 3), span = (4, 1), flag = wx.ALIGN_CENTER_HORIZONTAL)

    b = wx.StaticBox(self, wx.ID_ANY, "DC Motor Extruder")
    sbox = wx.StaticBoxSizer(b, wx.VERTICAL)
    sbox.AddSpacer((5, 5))

    k = 'DC_EXTRUDER'
    ch = self.addChoice(k, self.heaterNames, 0, 60, self.onChoice)
    sbox.Add(ch)
    sbox.AddSpacer((5, 5))

    k = 'DC_EXTRUDER_PWM'
    tc = self.addTextCtrl(k, 60, self.onTextCtrlInteger)
    sbox.Add(tc)
    sbox.AddSpacer((5, 5))

    sz.Add(sbox, pos = (8,3), flag = wx.ALIGN_CENTER_HORIZONTAL)

    k = 'MOVEBUFFER_SIZE'
    tc = self.addTextCtrl(k, labelWidth, self.onTextCtrlInteger)
    sz.Add(tc, pos = (1, 5))

    k = 'TH_COUNT'
    tc = self.addTextCtrl(k, labelWidth, self.onTextCtrlInteger)
    sz.Add(tc, pos = (2, 5))

    k = 'ENDSTOP_STEPS'
    tc = self.addTextCtrl(k, labelWidth, self.onTextCtrlInteger)
    sz.Add(tc, pos = (3, 5))

    k = 'PID_SCALE'
    tc = self.addTextCtrl(k, labelWidth, self.onTextCtrlInteger)
    sz.Add(tc, pos = (4, 5))

    k = 'TEMP_HYSTERESIS'
    tc = self.addTextCtrl(k, labelWidth, self.onTextCtrlFloat)
    sz.Add(tc, pos = (6, 5))

    k = 'TEMP_RESIDENCY_TIME'
    tc = self.addTextCtrl(k, labelWidth, self.onTextCtrlInteger)
    sz.Add(tc, pos = (7, 5))

    k = 'TEMP_EWMA'
    tc = self.addTextCtrl(k, labelWidth, self.onTextCtrlEWMA)
    sz.Add(tc, pos = (8, 5))

    self.SetSizer(sz)
    self.enableAll(False)

  def onTextCtrlEWMA(self, evt):
    self.assertModified(True)
    tc = evt.GetEventObject()
    name = tc.GetName()
    w = tc.GetValue().strip()
    if w == "":
      valid = True
    else:
      m = reFloat.match(w)
      if m:
        v = float(w)
        if v < 0.1 or v > 1.0:
          valid = False
        else:
          valid = True
      else:
        valid = False

    self.setFieldValidity(name, valid)

    if valid:
      tc.SetBackgroundColour(wx.SystemSettings_GetColour(wx.SYS_COLOUR_WINDOW))
    else:
      tc.SetBackgroundColour("pink")
    tc.Refresh()
    evt.Skip()

  def setHeaters(self, hlist):
    k = 'DC_EXTRUDER'
    v = self.choices[k].GetSelection()
    currentChoice = self.heaterNames[v]
    self.boardHeaters = [s[0] for s in hlist]
    self.heaterNames = [self.heaterNameNone] + self.boardHeaters
    self.choices[k].Clear()
    for h in self.heaterNames:
      self.choices[k].Append(h)
    self.choiceOptions[k] = self.heaterNames

    try:
      v = self.heaterNames.index(currentChoice)
    except:
      v = 0
      dlg = wx.MessageDialog(self,
                             "Printer: Miscellaneous tab:\nDC Extruder heater "
                             "\"%s\" not defined for this board. Please check."
                             % currentChoice, "Warning",
                             wx.OK + wx.ICON_WARNING)

      dlg.ShowModal()
      dlg.Destroy()

    self.choices[k].SetSelection(v)

  def setOriginalHeater(self, h):
    k = 'DC_EXTRUDER'
    if h and h.startswith("HEATER_"):
      hname = h[len("HEATER_"):]
    else:
      hname = h
    if len(self.boardHeaters) != 0:
      if hname not in self.boardHeaters:
        dlg = wx.MessageDialog(self,
                               "Printer: Miscellaneous tab:\nDC Extruder "
                               "heater \"%s\" not defined for this board. "
                               "Please check."
                               % hname, "Warning", wx.OK + wx.ICON_WARNING)

        dlg.ShowModal()
        dlg.Destroy()
      self.heaterNames = [self.heaterNameNone] + self.boardHeaters
    else:
      self.heaterNames = [self.heaterNameNone]
      if h and h != self.heaterNameNone:
        self.heaterNames.append(hname)
    self.choices[k].Clear()
    for ht in self.heaterNames:
      self.choices[k].Append(ht)
    self.choiceOptions[k] = self.heaterNames
    if hname:
      try:
        v = self.heaterNames.index(hname)
      except:
        v = 0
    else:
      v = 0
    self.choices[k].SetSelection(v)

  def insertValues(self, cfgValues):
    self.assertValid(True)
    for k in self.fieldValid.keys():
      self.fieldValid[k] = True

    for k in self.checkBoxes.keys():
      if k in cfgValues.keys() and cfgValues[k]:
        self.checkBoxes[k].SetValue(True)
      else:
        self.checkBoxes[k].SetValue(False)

    for k in self.textControls.keys():
      if k in cfgValues.keys():
        self.textControls[k].SetValue(str(cfgValues[k]))
      else:
        self.textControls[k].SetValue("")

    self.setChoice('REFERENCE', cfgValues, self.defaultRef)

    self.assertModified(False)
    self.enableAll(True)

  def getValues(self):
    result = Page.getValues(self)

    k = 'STEP_INTERRUPT_INTERRUPTIBLE'
    cb = self.checkBoxes[k]
    if cb.IsChecked():
      result[k] = "1"
    else:
      result[k] = "0"

    k = "DC_EXTRUDER"
    s = self.choices[k].GetSelection()
    v = self.choiceOptions[k][s]
    if v == self.heaterNameNone:
      result[k] = ""
    else:
      result[k] = "HEATER_%s" % v

    return result
